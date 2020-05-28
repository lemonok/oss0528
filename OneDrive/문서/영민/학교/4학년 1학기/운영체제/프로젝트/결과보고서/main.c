/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2013  Bruno Meneguello

  This program can be distributed under the terms of the GNU GPL.

  gcc -Wall skelfs.c `pkg-config fuse --cflags --libs` -o skelfs
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

/* superblock 과 마찬가지. 현재 파일의 개수, 최대 개수, 현재 넣을수있는 빈공간의 위치 */
int file_cnt = 2;
int max_file_num = 1000;
int filepos = 2;

/* descriptor가 참조되고 있는지 여부 */
int fdcnt[1000] = {0, };

/* file descriptor 구조체. stat에 있는 대부분의 값을 가지고 있다. */
typedef struct FileDescriptor{
    mode_t mode;
    nlink_t nlink;
    uid_t uid;
    gid_t gid;
    off_t size;
    time_t atime;
    time_t mtime;
    time_t ctime;
    char data[1024];
}FileDesc;

FileDesc *fi_de;

/* directory entry를 표현하는 구조체, 파일명과 해당되는 file descriptor 번호를 가지고 있음. */
typedef struct DirectoryEntry{
    char *name;
    int fdnum;
    struct DirectoryEntry *parent;
    struct DirectoryEntry *head_child;
    struct DirectoryEntry *left_sib;
    struct DirectoryEntry *right_sib;
}Dentry;

/* 기본적으로 mount 되면 지정될 entry,   1번 descriptor가 해당된다 */
Dentry root;

Dentry *getFile(char *path);
void setStat(struct stat *buf, int fd);
void makeDentry(const char *path, Dentry *temp, char* name, mode_t mode, int select);
void filePosInc();
void destroyEntry(Dentry *t);

/* directory entry 내에서 path에 해당되는 file이 있는지 확인하는 함수 */
Dentry *getFile(char *path){
    printf("getFile : \n");
    /* "/" 로 문자열을 나눔. 단계별로 접근하기 위함. */
    char *ptr = strtok(path, "/");
    Dentry *temp = root.head_child; // root의 head child부터 시작.
    Dentry *ptemp;
    if(temp == NULL) // root directory 밑에 아무것도 없으면
        return NULL;
    int find = 0;
    while(ptr != NULL){ // 주어진 path의 모든 이름들을 단계별로 다 검사할 때까지
        find = 0;
        printf("%s\n",ptr);
        if(temp != NULL) // 현재 보고있는 디렉토리 엔트리의 경로가 존재하면
            ptemp = temp;
        while(temp != NULL){ // 디렉토리 엔트리가 존재하면
            if(strcmp(ptr,temp->name) == 0){ // 이름이 같으면, 즉 중간 파일을 찾으면
                find = 1;
                break;
            }else{
                temp = temp->right_sib; // 같지 않으면 옆의 형제 노드들도 봄.
            }
        }
        ptr = strtok(NULL,"/"); // 다음 단계의 파일을 확인
        if(temp == NULL) // 현재 해당되는 단계의 디렉토리를 다 참조했는데도 파일이 없었으므로 계속 break.
            break;
        else{
            ptemp = temp;    // 파일이 존재하면 다음 단계로 넘어감.
            temp = temp->head_child;
        }
    }
    /* path내의 모든 파일을 다 확인했을 때 마지막에 find = 1 이었으면 끝까지 다 동일한거이므로 ptemp 를 반환 */
    if(find == 0){
        printf("no file\n");
        return NULL;
    }else{
        printf("file get %s\n",ptemp->name);
        return ptemp;
    }
}


/* buf 를 fd의 정보들로 채워주는 함수 */
void setStat(struct stat *buf, int fd){
    buf->st_mode = fi_de[fd].mode;
    buf->st_nlink = fi_de[fd].nlink;
    buf->st_ino = fd;
    buf->st_uid = fi_de[fd].uid;
    buf->st_gid = fi_de[fd].gid;
    buf->st_size = fi_de[fd].size;
    buf->st_atime = fi_de[fd].atime;
    buf->st_mtime = fi_de[fd].mtime;
    buf->st_ctime = fi_de[fd].ctime;

};

/* 새로운 directory entry를 만들어주는 함수 */
void makeDentry(const char *path, Dentry *temp, char* name, mode_t mode, int select){
    //select = 0 - reg , 1 - dir , 2 - symbolic link
    // 부모 노드가 temp로 주어지고, 이름,모드, 어떤 파일로 만들지가 주어진다.
    char *pn = (char*)malloc(sizeof(char)*(strlen(name)+1));

    strcpy(pn,name);
    /* 새로운 entry를 만들고 모든 값을 해당값, 기본값으로 바꿔준다 */
    Dentry * child = (Dentry*)malloc(sizeof(Dentry));
    child->name = pn;
    child->fdnum = filepos;
    child->parent = temp;
    child->head_child = NULL;
    child->left_sib = NULL;
    child->right_sib = NULL;

    file_cnt++; // 새로운 file을 만들었으므로.
    if(select == 1){  // directory의 default link는 2.
        fi_de[filepos].mode = S_IFDIR | mode;
        fi_de[filepos].nlink = 2;
    }else if(select == 0){
        fi_de[filepos].mode = S_IFREG | mode;
        fi_de[filepos].nlink = 1;
    }else{
        fi_de[filepos].mode = S_IFLNK | mode;
        fi_de[filepos].nlink = 1;
    }

    /* 만들어진 시간을 기입, size를 0으로 초기화, uid,gid 삽입 */
    struct fuse_context *fc = fuse_get_context();

    time_t current;
    time(&current);

    fi_de[filepos].size = 0;
    if(select == 2){
        fi_de[filepos].size = strlen(path);
        strcpy(fi_de[filepos].data,path);
    }
    fi_de[filepos].uid = fc->uid;
    fi_de[filepos].gid = fc->gid;
    fi_de[filepos].atime = current;
    fi_de[filepos].mtime = current;
    fi_de[filepos].ctime = current;

    if(select == 1) // directory 는 부모의 nlink를 1증가시켜줌.
        fi_de[temp->fdnum].nlink++;

    if(temp->head_child == NULL){ // 아직 자식이 없으면
        temp->head_child = child;
    }else{ // 있는경우 비어잇는 형제 노드 위치에 넣어줌.
        Dentry *temp1;
        Dentry *latemp;
        temp1 = temp->head_child;
        latemp = temp1;
        while(temp1 != NULL){
            latemp = temp1;
            temp1 = temp1->right_sib;
        }
        latemp->right_sib = child;
        child->left_sib = latemp;
    }
    /* 비어있는 file descriptor를 가리키는 위치를 바꿔줌 */
    filePosInc();

}

/* 현재 비어있는 file descriptor의 위치를 찾는 함수, nlink가 0인곳을 비었다고 가정 */
void filePosInc(){
    while(1){
        filepos++;
        if(filepos >= max_file_num) // file 갯수 1000개이므로.
            filepos = 2;
        if(fi_de[filepos].nlink == 0)
            break;
    }
}

/** Get file attributes.
 *
   st_ino filed는 mount시에 use_ino 옵션을 주지않으면 무시되고 순차적으로 계산되서 입력됨 .
   hard_link , soft_link 확인 위해서는 use_ino 옵션주고 할 필요성이 있음.
 */

static int fs_getattr(const char *path, struct stat *buf) {

    int res = 0;
    memset(buf, 0, sizeof(struct stat));
    printf("getattr - path : %s \n",path);
    if(strcmp(path,"/") == 0){ // root directory라면 그냥 1번 descriptor 정보 줌.
        setStat(buf,1);

    }else{ // 아닌 경우
        char newpath[1024]; // path가 const 이므로 복사해서 넘겨줌 [strtok을 쓰기 위해서 ]
        strcpy(newpath,path);
        newpath[strlen(path)] = '\0';

        Dentry *temp = getFile(newpath); // path에 해당되는 file의 pointer를 얻음.
        if(temp == NULL){ // 없는 경우
            return -ENOENT;
        }else{ // 있는 경우 그 directory entry에 해당되는 fd의 정보를 넘김.
            printf("path - %s , fdnum = %d\n",path,temp->fdnum);
            setStat(buf,temp->fdnum);
            printf("ino %d \n",buf->st_ino);
        }
    }
    return 0;
}


/** Create a file node
 *
 * This is called for creation of all non-directory, non-symlink
 * nodes.  If the filesystem defines a create() method, then for
 * regular files that will be called instead.
 */
static int fs_mknod(const char *path, mode_t mode, dev_t rdev) {

    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    printf("mknod - path : %s newpath - %s\n",path, newpath);
    char name[256];

    // 루트말고 /가 존재하면 다른 방식으로 진행.
    int i;
    for(i = strlen(path)-1; i>=0; i--){
        if(path[i] == '/')
            break;
    }
    if(i > 0){ // 루트 말고 디렉토리 존재.
        strncpy(newpath,path,i);
        newpath[i] = '\0';
        printf("except name - %s\n",newpath);
        Dentry * temp = getFile(newpath); // 파일명을 뺀 자기위의 디렉토리까지 탐색
        if(temp == NULL) // directory가 없으면
            return -ENOENT;
        if((fi_de[temp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOTDIR; // 상위 파일이 directory가 아님.
        int j;
        for(j = i+1; j<strlen(path); j++){
            name[j-i-1] = path[j];
        }
        name[strlen(path)-i-1] = '\0';
        printf("name %s\n",name);
        makeDentry(path,temp,name,mode,0); // 새로운 entry 및 fd 를 만듬.

    }else{ // 루트만 존재.
        strcpy(newpath,path);
        newpath[strlen(path)] = '\0';
        makeDentry(path,&root,newpath+1,mode,0);// 새로운 entry 및 fd 를 만듬.
    }


    return 0;
}

/** Create a directory
 *
 * Note that the mode argument may not have the type specification
 * bits set, i.e. S_ISDIR(mode) can be false.  To obtain the
 * correct directory type bits use  mode|S_IFDIR
 * */
static int fs_mkdir(const char *path, mode_t mode) {
    /* 기본적인 동작방식은 mknod와 동일하다 */
    char *ptr;
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    printf("mkdir - path : %s newpath - %s\n",path, newpath);
    char name[256];


    int i;
    for(i = strlen(path)-1; i>=0; i--){
        if(path[i] == '/')
            break;
    }
    if(i > 0){ // 루트 말고 디렉토리 존재.
        strncpy(newpath,path,i);
        newpath[i] = '\0';
        printf("except name - %s\n",newpath);
        Dentry * temp = getFile(newpath);
        if(temp == NULL) // directory가 없으면
            return -ENOENT;
        if((fi_de[temp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOENT; // 상위 파일이 directory가 아님.
        int j;
        for(j = i+1; j<strlen(path); j++){
            name[j-i-1] = path[j];
        }
        name[strlen(path)-i-1] = '\0';
        printf("name %s\n",name);
        /* path 경로에 해당되는 temp를 부모로 갖고 name을 이름으 갖는 [/asd/hi/c -> c] directory 생성.*/
        makeDentry(path,temp,name,mode,1);


    }else{ // 루트만 존재.
        strcpy(newpath,path);
        newpath[strlen(path)] = '\0';
        /* path 경로에 해당되는 root를 부모로 갖고 newpath의 [1] 인자부터의 이름을 갖는 [/asd -> asd] directory 생성.*/
        makeDentry(path,&root,newpath+1,mode,1);

    }

    return 0;
}

/** Remove a file */
static int fs_unlink(const char *path) {
    printf("unlink - path : %s \n",path);
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';

    Dentry * temp = getFile(newpath);

    if(temp == NULL)
        return -ENOENT;

    if((fi_de[temp->fdnum].mode & S_IFDIR) == S_IFDIR) // 디렉토리라면 rmdir에서 처리해주면되니까
        return -EISDIR;

    if(fdcnt[temp->fdnum] > 0) // 현재 참조중인 파일이라면.
        return -EBUSY;

    if(temp->left_sib != NULL){ // head child가 아닌 경우
        /* 자신의 양옆 entry를 처리해준다. */
        temp->left_sib->right_sib = temp->right_sib;
        if(temp->right_sib != NULL)
            temp->right_sib->left_sib = temp->left_sib;
    }else {// head child 인 경우
        /* 자신의 부모와 오른쪽의 entry를 처리해준다 */
        temp->parent->head_child = temp->right_sib;
        if(temp->right_sib != NULL)
            temp->right_sib->left_sib = temp->left_sib;
    }


    printf("mode, s_iflnk%o  %o\n",fi_de[temp->fdnum].mode, S_IFLNK);
    /* soft link 가 아닌 경우 nlink를 1 빼줘야함. soft link인 경우에는 nlink를 0으로 만듦.*/
    if((fi_de[temp->fdnum].mode & S_IFLNK) != S_IFLNK) //
        fi_de[temp->fdnum].nlink--;
    else
        fi_de[temp->fdnum].nlink = 0;
    /* 현재 파일 숫자를 감소 */
    file_cnt--;
    free(temp);
    return 0;
}

/** Remove a directory */
static int fs_rmdir(const char *path) {
    /* unlink와 동작방식은 같다 */
    printf("rmdir - path : %s \n",path);
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';

    Dentry * temp = getFile(newpath);

    if(temp == NULL)
        return -ENOENT;

    if((fi_de[temp->fdnum].mode & S_IFDIR) == 0)
        return -ENOTDIR;

    if(temp->head_child != NULL) // 디렉토리 내부에 파일이 있는 경우
        return -ENOTEMPTY;
    if(temp->left_sib != NULL){ // head child가 아닌 경우
        temp->left_sib->right_sib = temp->right_sib;
        if(temp->right_sib != NULL)
            temp->right_sib->left_sib = temp->left_sib;
    }else{ // head child 인 경우
        temp->parent->head_child = temp->right_sib;
        if(temp->right_sib != NULL)
            temp->right_sib->left_sib = temp->left_sib;
    }
    file_cnt--;
    fi_de[temp->parent->fdnum].nlink--;
    fi_de[temp->fdnum].nlink = 0;

    free(temp);

    return 0;
}

/** Create a symbolic link */
static int fs_symlink(const char *linkname, const char *path) {
    /* symbolic link는 바로가기와 비슷한 개념이다. hard link와는 달리 inode를 따로 생성하지만
    이 inode는 단순히 다른 inode를 가리킬 뿐이다. 따라서 data에 가리키는 path를 담고
    접근권한은 0777로 주며, nlink는 1으로 한다. 또한 hard link는 가리키는 inode의 nlink값을 증가시키지만
    symbolic link의 경우에는 변화 없다. */
    printf("symlink - name : %s path : %s \n",linkname, path);
    char npath[1024];
    char *pn;
    strcpy(npath,path);
    npath[strlen(path)] = '\0';

    Dentry *newtemp = getFile(npath);
    if(newtemp != NULL) // 이미 이러한 파일명이 있는 경우
        return -EEXIST;

    char name[256];

    char opath[1024];
    strcpy(opath,linkname);
    opath[strlen(linkname)] = '\0';


    Dentry *oldtemp = getFile(opath);
    if(oldtemp == NULL) // 심볼릭 링크할 파일이 없는 경우
        return -ENOENT;

    int i;
    for(i = strlen(path)-1; i>=0; i--){
        if(path[i] == '/')
            break;
    }
    if(i > 0){ // 루트 말고 디렉토리 존재.
        strncpy(npath,path,i);
        npath[i] = '\0';
        printf("except name - %s\n",npath);
        newtemp = getFile(npath);
        if(newtemp == NULL) // directory가 없으면
            return -ENOENT;
        if((fi_de[newtemp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOENT; // 상위 파일이 directory가 아님.
        int j;
        for(j = i+1; j<strlen(path); j++){
            name[j-i-1] = npath[j];
        }
        name[strlen(path)-i-1] = '\0';
        printf("name %s\n",name);

        makeDentry(linkname,newtemp,name,0777,2);

    }else{ // 루트만 존재.
        strcpy(npath,path);
        npath[strlen(path)] = '\0';

        makeDentry(linkname,&root,npath+1,0777,2);

    }

    return 0;
}

static int fs_readlink(const char *path, char *buf, size_t size){
    /* symbolic link가 가리키는 곳을 따라갈 수 있게 해주는 readlink 이다 */
    printf("readlink : %s %d\n",path,size);
    char npath[1024];

    strcpy(npath,path);
    npath[strlen(path)] = '\0';

    Dentry *newtemp = getFile(npath);

    if(newtemp == NULL)
        return -ENOENT;
    /* symbolic link가 가지고 있는 data 파일에 있는 path를 size만큼 그대로 넘겨준다 */
    strncpy(buf,fi_de[newtemp->fdnum].data,fi_de[newtemp->fdnum].size);
    buf[fi_de[newtemp->fdnum].size] = '\0';
    return 0;
}

/** Rename a file */
static int fs_rename(const char *oldpath, const char *newpath) {
    /* 유닉스에서 rename은 mv command를 뜻한다.
    동일한 디렉토리 내에서 rename은 단순한 파일명변경이며
    cp와는 다르게 새롭게 엔트리를 생성하거나 삭제하는 과정이 일어나는 것이 아닌
    단순히 entry만 옮겨주면 된다. */
    printf("rename %s %s\n",oldpath,newpath);
    char npath[1024];
    char *pn;
    strcpy(npath,newpath);
    npath[strlen(newpath)] = '\0';

    Dentry *newtemp = getFile(npath);
    if(newtemp != NULL)
        return -EEXIST;

    char name[256];

    char opath[1024];
    strcpy(opath,oldpath);
    opath[strlen(oldpath)] = '\0';


    Dentry *oldtemp = getFile(opath);
    if(oldtemp == NULL){
        return -ENOENT;
    }

    int i;
    for(i = strlen(newpath)-1; i>=0; i--){
        if(newpath[i] == '/')
            break;
    }
    if(i > 0){ // 루트 말고 디렉토리 존재.
        strncpy(npath,newpath,i);
        npath[i] = '\0';
        printf("except name - %s\n",npath);
        newtemp = getFile(npath);
        if(newtemp == NULL) // directory가 없으면
            return -ENOENT;
        if((fi_de[newtemp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOTDIR; // 상위 파일이 directory가 아님.
        int j;
        for(j = i+1; j<strlen(newpath); j++){
            name[j-i-1] = npath[j];
        }
        name[strlen(newpath)-i-1] = '\0';
        printf("name %s\n",name);

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,name);
    }else{ // 루트만 존재.
        strcpy(npath,newpath);
        npath[strlen(newpath)] = '\0';

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,npath+1);
        newtemp = &root;


    }

    if(oldtemp->parent == newtemp){ // 동일한 directory내에 있다면 이름 변경만 하면 됨.
        oldtemp->name = pn;
    }else{ // 다르면 directory entry 구조 변경해줘야함.
        /* 파일명 변경 */
        oldtemp->name = pn;
        /* 원래 있던 양옆 엔트리와 부모 디렉토리에 대한 처리 */
        if(oldtemp->left_sib != NULL){ // head child가 아닌 경우
            oldtemp->left_sib->right_sib = oldtemp->right_sib;
            if(oldtemp->right_sib != NULL)
                oldtemp->right_sib->left_sib = oldtemp->left_sib;
        }else {// head child 인 경우
            oldtemp->parent->head_child = oldtemp->right_sib;
            if(oldtemp->right_sib != NULL)
                oldtemp->right_sib->left_sib = oldtemp->left_sib;
        }
        /* 옮기는 파일이 디렉토리인 경우 상위 디렉토리의 nlink를 1 감소시켜주어야함. */
        if((fi_de[oldtemp->fdnum].mode & S_IFDIR) == S_IFDIR){
            fi_de[oldtemp->parent->fdnum].nlink--;
        }
        /* 새롭게 directory entry를 옮기는 과정 */
        oldtemp->parent = newtemp;
        oldtemp->left_sib = NULL;
        oldtemp->right_sib = newtemp->head_child;
        if(newtemp->head_child != NULL)
            newtemp->head_child->left_sib = oldtemp;
        newtemp->head_child = oldtemp;
        if((fi_de[oldtemp->fdnum].mode & S_IFDIR) == S_IFDIR){
            fi_de[oldtemp->parent->fdnum].nlink++;
        }
    }

    return 0;
}

/** Create a hard link to a file */
static int fs_link(const char *oldpath, const char *newpath) {
    /* hard link를 나타내며 이 경우에는 directory entry는 생성하지만
    file descriptor는 생성하지 않는다. 다만, 가르키는 file descriptor의 nlink를 1 증가시킨다. */
    printf("link - path : %s  %s\n",oldpath,newpath);

    char npath[1024];
    char *pn;
    strcpy(npath,newpath);
    npath[strlen(newpath)] = '\0';

    Dentry *newtemp = getFile(npath);
    if(newtemp != NULL)
        return -EEXIST;

    char name[256];

    char opath[1024];
    strcpy(opath,oldpath);
    opath[strlen(oldpath)] = '\0';


    Dentry *oldtemp = getFile(opath);
    if(oldtemp == NULL){
        return -ENOENT;
    }


    int i;
    for(i = strlen(newpath)-1; i>=0; i--){
        if(newpath[i] == '/')
            break;
    }
    if(i > 0){ // 루트 말고 디렉토리 존재.
        strncpy(npath,newpath,i);
        npath[i] = '\0';
        printf("except name - %s\n",npath);
        newtemp = getFile(npath);
        if(newtemp == NULL) // directory가 없으면
            return -ENOENT;
        if((fi_de[newtemp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOENT; // 상위 파일이 directory가 아님.
        int j;
        for(j = i+1; j<strlen(newpath); j++){
            name[j-i-1] = npath[j];
        }
        name[strlen(newpath)-i-1] = '\0';
        printf("name %s\n",name);

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,name);
    }else{ // 루트만 존재.
        strcpy(npath,newpath);
        npath[strlen(newpath)] = '\0';

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,npath+1);
        newtemp = &root;


    }
    /* hard link 이므로 directory entry만 생성하고 file descriptor는 새로 만들지않고 원래 있는걸 가르킴. */
    Dentry * child = (Dentry*)malloc(sizeof(Dentry));
    child->name = pn;
    child->fdnum = oldtemp->fdnum;
    printf("oldtemp fdnum : %d %d \n ",oldtemp->fdnum, child->fdnum);
    child->parent = newtemp;
    child->head_child = NULL;
    child->left_sib = NULL;
    child->right_sib = NULL;

    /* 다만 file_Cnt 는 늘려주고, 원래의 nlink 를 1 더해준다. */
    file_cnt++;
    fi_de[oldtemp->fdnum].nlink++;



    if(newtemp->head_child == NULL){
        newtemp->head_child = child;
    }else{
        Dentry *temp1;
        Dentry *latemp;
        temp1 = newtemp->head_child;
        latemp = temp1;
        while(temp1 != NULL){
            latemp = temp1;
            temp1 = temp1->right_sib;
        }
        latemp->right_sib = child;
        child->left_sib = latemp;
    }

    return 0;
}

/** Change the permission bits of a file */
static int fs_chmod(const char *path, mode_t mode) {
    /* permission bit를 바꾼다. 이 접근하는 프로그램이 이 파일의 owner인지 확인해주어야한다. */
    printf("chmod - path : %s \n",path);
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL)
        return -ENOENT;

    struct fuse_context *fc = fuse_get_context();
    if(fc->uid != fi_de[temp->fdnum].uid) // 명령을 실행한 사람이 파일의 소유주가 아니면
        return -EACCES;
    printf("mode %d %d\n",fi_de[temp->fdnum].mode, mode);

    fi_de[temp->fdnum].mode = mode;

    return 0;
}

/** File open operation
 *
 * No creation (O_CREAT, O_EXCL) and by default also no
 * truncation (O_TRUNC) flags will be passed to open(). If an
 * application specifies O_TRUNC, fuse first calls truncate()
 * and then open(). Only if 'atomic_o_trunc' has been
 * specified and kernel version is 2.6.24 or later, O_TRUNC is
 * passed on to open.
 *
 * Unless the 'default_permissions' mount option is given,
 * open should check if the operation is permitted for the
 * given flags. Optionally open may also return an arbitrary
 * filehandle in the fuse_file_info structure, which will be
 * passed to all file operations.
 *
 * Changed in version 2.2
 */
static int fs_open(const char *path, struct fuse_file_info *fi) {
    /* 접근 파일에 대해 접근권한을 확인하고 열어준다. */
    printf("open - path : %s \n",path);
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL)
        return -ENOENT;
    if(fi_de[temp->fdnum].mode & S_IFDIR == S_IFDIR)
        return -EISDIR; //  DIRECTORY이면.
    /* fdcnt를 사용하여 참조하고 있는지 여부를 나타낸다 */
    fdcnt[temp->fdnum]++; // 파일을 열었으므로 count 늘려줌.

    if((fi->flags & 3) == O_RDONLY){ // 파일에 접근하는 플래그 확인
        if( (fi_de[temp->fdnum].mode & 0400) == 0)    // owner 권한을 접근플래그랑 확인.
            return -EACCES;
    }else if((fi->flags & 3) == O_WRONLY){
        if( (fi_de[temp->fdnum].mode & 0200) == 0)
            return -EACCES;
    }else if((fi->flags & 3) == O_RDWR){
        if( (fi_de[temp->fdnum].mode & 0600) == 0)
            return -EACCES;
    }
    fi->fh = temp->fdnum; // handler에 현재 descriptor의 number를 넘겨줌.
    return 0;
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.     An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 *
 * Changed in version 2.2
 */
static int fs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf("read - path : %s \n",path);
    /* file을 읽어들이는 함수 부분. */
    size_t len;
    (void) fi;
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL) // 존재하는 파일인지 확인
        return -ENOENT;
    // 이 파일시스템에서 저장할 수 있는 데이터의 크기는 1KB 이므로, 그 이하만 읽게 한다.
    len = fi_de[temp->fdnum].size;
    printf("len %d offset %d, size %d \n",len,offset,size);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, fi_de[temp->fdnum].data + offset, size);
        printf("read text : %s \n",fi_de[temp->fdnum].data);
    } else
        size = 0;
    /* read와 write는 읽은 크기만큼을 반환한다 */
    return size;
}

/** Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.     An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 *
 * Changed in version 2.2
 */
static int fs_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf("write - path : %s \n",path);
    /* write를 진행하는 함수이다 */
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL)
        return -ENOENT;
    size_t len = sizeof(fi_de[temp->fdnum].data);
    printf("len %d offset %d, size %d \n",len,offset,size);
    /* 1KB가 넘는 파일은 적을수 없음 */
    if(size>len) // 공간 부족.
        return -EIO;

    memcpy(fi_de[temp->fdnum].data + offset, buf, size);
    fi_de[temp->fdnum].size = size;
    /* read와 write는 읽은 크기만큼을 반환한다 */
    return size;
}


/** Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor.  So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors.  Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().    This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls.    It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally.  Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 *
 * Changed in version 2.2
 */
static int fs_flush(const char *path, struct fuse_file_info *fi) {
    /* 한번의 close가 일어날 때마다 발생하며, open 에서 fi 에 넘겨준 fh에 해당되는 fdcnt를 1 감소시켜준다.
    */
    printf("flush - path : %s fh %d\n",path,fi->fh);
    if(fdcnt[fi->fh] > 0) fdcnt[fi->fh]--;
    return 0;
}

/** Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.     It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 *
 * Changed in version 2.2
 */
static int fs_release(const char *path, struct fuse_file_info *fi) {
    /* 한 파일에 대해 모든 참조가 끝났을 때 불리는 함수로써, fdcnt를 0으로 만들어 준다. */
    printf("release - path : %s fh %d\n",path,fi->fh);
    fdcnt[fi->fh] = 0;
    return 0;
}

/** Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 *
 * Introduced in version 2.3
 */
static int fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    /* 한 directory file 내에 포함된 모든 파일들을 읽어들이는 함수이다.
    읽은 내용을 buf에 filler 함수를 통해 적어서 반환한다. */
    printf("readdir - path : %s \n",path);
    (void) offset;
    (void) fi;
    char newpath[1024];
    Dentry *temp;
    struct stat ns;
    memset(&ns,0,sizeof(ns));

    /* filler(buf, name, &stat, 0);
    buf에 name[파일명]을 적고 stat 구조체를 넘겨줌으로써 ls 와 같은 명령어를 쳤을 때
    단순히 파일명만 보이는 것이 아닌 파일명과 파일에 해당되는 descriptor 정보가 나타난다
    기본적으로 .과 ..는 유닉스에서 기본적으로 포함되어야 한다.
    '.' - 자기 자신  '..' - 부모 */
    if (strcmp(path, "/") == 0){ // path가 root라면
        temp = root.head_child;
        setStat(&ns, 1); // root에 해당되는 1번 descriptor file.
        filler(buf, ".", &ns, 0);
        filler(buf, "..", NULL, 0);

    }else{ // root가 아니라면
        strcpy(newpath,path);
        newpath[strlen(path)] = '\0';
        temp = getFile(newpath);
        if(temp == NULL)
            return -ENOENT;
        setStat(&ns, temp->fdnum);
        filler(buf, ".", &ns, 0);
        memset(&ns,0,sizeof(ns));
        setStat(&ns, temp->parent->fdnum);
        filler(buf, "..", &ns, 0);
        temp = temp->head_child;

    }

    /* 자기 자식의 파일 모두를 출력한다. [temp = head_child 부터 null이 될 때까지 오른쪽 형제 엔트리로 이동 ] */
    while(temp != NULL){
        memset(&ns,0,sizeof(ns));
        setStat(&ns, temp->fdnum);

        filler(buf, temp->name, &ns, 0);
        printf("temp fdnum  :%d\n",ns.st_ino);
        printf("%s\t",temp->name);
        temp = temp->right_sib;
    }
    printf("\n");

    return 0;
}


/**
 * Initialize filesystem
 *
 * The return value will passed in the private_data field of
 * fuse_context to all file operations and as a parameter to the
 * destroy() method.
 *
 * Introduced in version 2.3
 * Changed in version 2.6
 */
static void *fs_init(struct fuse_conn_info *conn) {
    /* 시작시에 root에 관한 초기값과 file descriptor 배열 생성 */
    root.name = "root";
    root.fdnum = 1;
    root.parent = NULL;
    root.head_child = NULL;
    root.left_sib = NULL;
    root.right_sib = NULL;
    fi_de = (FileDesc*)malloc(sizeof(FileDesc)*max_file_num);

    fi_de[1].mode = S_IFDIR | 0777;
    fi_de[1].nlink = 2;

    struct fuse_context *fc;
    fc = fuse_get_context();

    fi_de[1].uid = fc->uid;
    fi_de[1].gid = fc->gid;

    time_t current_time;
    time( &current_time );

    fi_de[1].atime = current_time;
    fi_de[1].ctime = current_time;
    fi_de[1].mtime = current_time;
    printf("uid,gid %d %d\n",fc->uid,fc->gid);
    return 0;
}

/**
 * Clean up filesystem
 *
 * Called on filesystem exit.
 *
 * Introduced in version 2.3
 */

void destroyEntry(Dentry *t){
    /* directory entry에 관한 모든 메모리를 재귀함수로 해제 */
    if(t == NULL)
        return;
    Dentry *temp = t->right_sib;
    while(temp != NULL){// 형제 노드 제거
        destroyEntry(temp);
        temp = temp->right_sib;
    }
    destroyEntry(t->head_child); //자식 제거
    free(t);

}

static void fs_destroy(void *data) {
    printf("destroy");
    /* 메모리 해제 과정이 이루어진다. */
    int i;
    for(i=0; i<max_file_num; i++){
        if(fi_de[i].data != NULL)
            free(fi_de[i].data);
    }
    free(fi_de);

    destroyEntry(root.head_child);
}

static struct fuse_operations fs_oper = {
    .getattr     = fs_getattr,
    .mknod       = fs_mknod,
    .mkdir       = fs_mkdir,
    .unlink      = fs_unlink,
    .rmdir       = fs_rmdir,
    .symlink     = fs_symlink,
    .rename      = fs_rename,
    .link        = fs_link,
    .readlink    = fs_readlink,
    .chmod       = fs_chmod,
    .open        = fs_open,
    .read        = fs_read,
    .write       = fs_write,
    .flush       = fs_flush,
    .release     = fs_release,
    .readdir     = fs_readdir,
    .init        = fs_init,
    .destroy     = fs_destroy,
};

int main(int argc, char *argv[]) {




    printf("initial\n");

    return fuse_main(argc, argv, &fs_oper, NULL);
}
