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

/* superblock �� ��������. ���� ������ ����, �ִ� ����, ���� �������ִ� ������� ��ġ */
int file_cnt = 2;
int max_file_num = 1000;
int filepos = 2;

/* descriptor�� �����ǰ� �ִ��� ���� */
int fdcnt[1000] = {0, };

/* file descriptor ����ü. stat�� �ִ� ��κ��� ���� ������ �ִ�. */
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

/* directory entry�� ǥ���ϴ� ����ü, ���ϸ�� �ش�Ǵ� file descriptor ��ȣ�� ������ ����. */
typedef struct DirectoryEntry{
    char *name;
    int fdnum;
    struct DirectoryEntry *parent;
    struct DirectoryEntry *head_child;
    struct DirectoryEntry *left_sib;
    struct DirectoryEntry *right_sib;
}Dentry;

/* �⺻������ mount �Ǹ� ������ entry,   1�� descriptor�� �ش�ȴ� */
Dentry root;

Dentry *getFile(char *path);
void setStat(struct stat *buf, int fd);
void makeDentry(const char *path, Dentry *temp, char* name, mode_t mode, int select);
void filePosInc();
void destroyEntry(Dentry *t);

/* directory entry ������ path�� �ش�Ǵ� file�� �ִ��� Ȯ���ϴ� �Լ� */
Dentry *getFile(char *path){
    printf("getFile : \n");
    /* "/" �� ���ڿ��� ����. �ܰ躰�� �����ϱ� ����. */
    char *ptr = strtok(path, "/");
    Dentry *temp = root.head_child; // root�� head child���� ����.
    Dentry *ptemp;
    if(temp == NULL) // root directory �ؿ� �ƹ��͵� ������
        return NULL;
    int find = 0;
    while(ptr != NULL){ // �־��� path�� ��� �̸����� �ܰ躰�� �� �˻��� ������
        find = 0;
        printf("%s\n",ptr);
        if(temp != NULL) // ���� �����ִ� ���丮 ��Ʈ���� ��ΰ� �����ϸ�
            ptemp = temp;
        while(temp != NULL){ // ���丮 ��Ʈ���� �����ϸ�
            if(strcmp(ptr,temp->name) == 0){ // �̸��� ������, �� �߰� ������ ã����
                find = 1;
                break;
            }else{
                temp = temp->right_sib; // ���� ������ ���� ���� ���鵵 ��.
            }
        }
        ptr = strtok(NULL,"/"); // ���� �ܰ��� ������ Ȯ��
        if(temp == NULL) // ���� �ش�Ǵ� �ܰ��� ���丮�� �� �����ߴµ��� ������ �������Ƿ� ��� break.
            break;
        else{
            ptemp = temp;    // ������ �����ϸ� ���� �ܰ�� �Ѿ.
            temp = temp->head_child;
        }
    }
    /* path���� ��� ������ �� Ȯ������ �� �������� find = 1 �̾����� ������ �� �����Ѱ��̹Ƿ� ptemp �� ��ȯ */
    if(find == 0){
        printf("no file\n");
        return NULL;
    }else{
        printf("file get %s\n",ptemp->name);
        return ptemp;
    }
}


/* buf �� fd�� ������� ä���ִ� �Լ� */
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

/* ���ο� directory entry�� ������ִ� �Լ� */
void makeDentry(const char *path, Dentry *temp, char* name, mode_t mode, int select){
    //select = 0 - reg , 1 - dir , 2 - symbolic link
    // �θ� ��尡 temp�� �־�����, �̸�,���, � ���Ϸ� �������� �־�����.
    char *pn = (char*)malloc(sizeof(char)*(strlen(name)+1));

    strcpy(pn,name);
    /* ���ο� entry�� ����� ��� ���� �ش簪, �⺻������ �ٲ��ش� */
    Dentry * child = (Dentry*)malloc(sizeof(Dentry));
    child->name = pn;
    child->fdnum = filepos;
    child->parent = temp;
    child->head_child = NULL;
    child->left_sib = NULL;
    child->right_sib = NULL;

    file_cnt++; // ���ο� file�� ��������Ƿ�.
    if(select == 1){  // directory�� default link�� 2.
        fi_de[filepos].mode = S_IFDIR | mode;
        fi_de[filepos].nlink = 2;
    }else if(select == 0){
        fi_de[filepos].mode = S_IFREG | mode;
        fi_de[filepos].nlink = 1;
    }else{
        fi_de[filepos].mode = S_IFLNK | mode;
        fi_de[filepos].nlink = 1;
    }

    /* ������� �ð��� ����, size�� 0���� �ʱ�ȭ, uid,gid ���� */
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

    if(select == 1) // directory �� �θ��� nlink�� 1����������.
        fi_de[temp->fdnum].nlink++;

    if(temp->head_child == NULL){ // ���� �ڽ��� ������
        temp->head_child = child;
    }else{ // �ִ°�� ����մ� ���� ��� ��ġ�� �־���.
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
    /* ����ִ� file descriptor�� ����Ű�� ��ġ�� �ٲ��� */
    filePosInc();

}

/* ���� ����ִ� file descriptor�� ��ġ�� ã�� �Լ�, nlink�� 0�ΰ��� ����ٰ� ���� */
void filePosInc(){
    while(1){
        filepos++;
        if(filepos >= max_file_num) // file ���� 1000���̹Ƿ�.
            filepos = 2;
        if(fi_de[filepos].nlink == 0)
            break;
    }
}

/** Get file attributes.
 *
   st_ino filed�� mount�ÿ� use_ino �ɼ��� ���������� ���õǰ� ���������� ���Ǽ� �Էµ� .
   hard_link , soft_link Ȯ�� ���ؼ��� use_ino �ɼ��ְ� �� �ʿ伺�� ����.
 */

static int fs_getattr(const char *path, struct stat *buf) {

    int res = 0;
    memset(buf, 0, sizeof(struct stat));
    printf("getattr - path : %s \n",path);
    if(strcmp(path,"/") == 0){ // root directory��� �׳� 1�� descriptor ���� ��.
        setStat(buf,1);

    }else{ // �ƴ� ���
        char newpath[1024]; // path�� const �̹Ƿ� �����ؼ� �Ѱ��� [strtok�� ���� ���ؼ� ]
        strcpy(newpath,path);
        newpath[strlen(path)] = '\0';

        Dentry *temp = getFile(newpath); // path�� �ش�Ǵ� file�� pointer�� ����.
        if(temp == NULL){ // ���� ���
            return -ENOENT;
        }else{ // �ִ� ��� �� directory entry�� �ش�Ǵ� fd�� ������ �ѱ�.
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

    // ��Ʈ���� /�� �����ϸ� �ٸ� ������� ����.
    int i;
    for(i = strlen(path)-1; i>=0; i--){
        if(path[i] == '/')
            break;
    }
    if(i > 0){ // ��Ʈ ���� ���丮 ����.
        strncpy(newpath,path,i);
        newpath[i] = '\0';
        printf("except name - %s\n",newpath);
        Dentry * temp = getFile(newpath); // ���ϸ��� �� �ڱ����� ���丮���� Ž��
        if(temp == NULL) // directory�� ������
            return -ENOENT;
        if((fi_de[temp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOTDIR; // ���� ������ directory�� �ƴ�.
        int j;
        for(j = i+1; j<strlen(path); j++){
            name[j-i-1] = path[j];
        }
        name[strlen(path)-i-1] = '\0';
        printf("name %s\n",name);
        makeDentry(path,temp,name,mode,0); // ���ο� entry �� fd �� ����.

    }else{ // ��Ʈ�� ����.
        strcpy(newpath,path);
        newpath[strlen(path)] = '\0';
        makeDentry(path,&root,newpath+1,mode,0);// ���ο� entry �� fd �� ����.
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
    /* �⺻���� ���۹���� mknod�� �����ϴ� */
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
    if(i > 0){ // ��Ʈ ���� ���丮 ����.
        strncpy(newpath,path,i);
        newpath[i] = '\0';
        printf("except name - %s\n",newpath);
        Dentry * temp = getFile(newpath);
        if(temp == NULL) // directory�� ������
            return -ENOENT;
        if((fi_de[temp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOENT; // ���� ������ directory�� �ƴ�.
        int j;
        for(j = i+1; j<strlen(path); j++){
            name[j-i-1] = path[j];
        }
        name[strlen(path)-i-1] = '\0';
        printf("name %s\n",name);
        /* path ��ο� �ش�Ǵ� temp�� �θ�� ���� name�� �̸��� ���� [/asd/hi/c -> c] directory ����.*/
        makeDentry(path,temp,name,mode,1);


    }else{ // ��Ʈ�� ����.
        strcpy(newpath,path);
        newpath[strlen(path)] = '\0';
        /* path ��ο� �ش�Ǵ� root�� �θ�� ���� newpath�� [1] ���ں����� �̸��� ���� [/asd -> asd] directory ����.*/
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

    if((fi_de[temp->fdnum].mode & S_IFDIR) == S_IFDIR) // ���丮��� rmdir���� ó�����ָ�Ǵϱ�
        return -EISDIR;

    if(fdcnt[temp->fdnum] > 0) // ���� �������� �����̶��.
        return -EBUSY;

    if(temp->left_sib != NULL){ // head child�� �ƴ� ���
        /* �ڽ��� �翷 entry�� ó�����ش�. */
        temp->left_sib->right_sib = temp->right_sib;
        if(temp->right_sib != NULL)
            temp->right_sib->left_sib = temp->left_sib;
    }else {// head child �� ���
        /* �ڽ��� �θ�� �������� entry�� ó�����ش� */
        temp->parent->head_child = temp->right_sib;
        if(temp->right_sib != NULL)
            temp->right_sib->left_sib = temp->left_sib;
    }


    printf("mode, s_iflnk%o  %o\n",fi_de[temp->fdnum].mode, S_IFLNK);
    /* soft link �� �ƴ� ��� nlink�� 1 �������. soft link�� ��쿡�� nlink�� 0���� ����.*/
    if((fi_de[temp->fdnum].mode & S_IFLNK) != S_IFLNK) //
        fi_de[temp->fdnum].nlink--;
    else
        fi_de[temp->fdnum].nlink = 0;
    /* ���� ���� ���ڸ� ���� */
    file_cnt--;
    free(temp);
    return 0;
}

/** Remove a directory */
static int fs_rmdir(const char *path) {
    /* unlink�� ���۹���� ���� */
    printf("rmdir - path : %s \n",path);
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';

    Dentry * temp = getFile(newpath);

    if(temp == NULL)
        return -ENOENT;

    if((fi_de[temp->fdnum].mode & S_IFDIR) == 0)
        return -ENOTDIR;

    if(temp->head_child != NULL) // ���丮 ���ο� ������ �ִ� ���
        return -ENOTEMPTY;
    if(temp->left_sib != NULL){ // head child�� �ƴ� ���
        temp->left_sib->right_sib = temp->right_sib;
        if(temp->right_sib != NULL)
            temp->right_sib->left_sib = temp->left_sib;
    }else{ // head child �� ���
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
    /* symbolic link�� �ٷΰ���� ����� �����̴�. hard link�ʹ� �޸� inode�� ���� ����������
    �� inode�� �ܼ��� �ٸ� inode�� ����ų ���̴�. ���� data�� ����Ű�� path�� ���
    ���ٱ����� 0777�� �ָ�, nlink�� 1���� �Ѵ�. ���� hard link�� ����Ű�� inode�� nlink���� ������Ű����
    symbolic link�� ��쿡�� ��ȭ ����. */
    printf("symlink - name : %s path : %s \n",linkname, path);
    char npath[1024];
    char *pn;
    strcpy(npath,path);
    npath[strlen(path)] = '\0';

    Dentry *newtemp = getFile(npath);
    if(newtemp != NULL) // �̹� �̷��� ���ϸ��� �ִ� ���
        return -EEXIST;

    char name[256];

    char opath[1024];
    strcpy(opath,linkname);
    opath[strlen(linkname)] = '\0';


    Dentry *oldtemp = getFile(opath);
    if(oldtemp == NULL) // �ɺ��� ��ũ�� ������ ���� ���
        return -ENOENT;

    int i;
    for(i = strlen(path)-1; i>=0; i--){
        if(path[i] == '/')
            break;
    }
    if(i > 0){ // ��Ʈ ���� ���丮 ����.
        strncpy(npath,path,i);
        npath[i] = '\0';
        printf("except name - %s\n",npath);
        newtemp = getFile(npath);
        if(newtemp == NULL) // directory�� ������
            return -ENOENT;
        if((fi_de[newtemp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOENT; // ���� ������ directory�� �ƴ�.
        int j;
        for(j = i+1; j<strlen(path); j++){
            name[j-i-1] = npath[j];
        }
        name[strlen(path)-i-1] = '\0';
        printf("name %s\n",name);

        makeDentry(linkname,newtemp,name,0777,2);

    }else{ // ��Ʈ�� ����.
        strcpy(npath,path);
        npath[strlen(path)] = '\0';

        makeDentry(linkname,&root,npath+1,0777,2);

    }

    return 0;
}

static int fs_readlink(const char *path, char *buf, size_t size){
    /* symbolic link�� ����Ű�� ���� ���� �� �ְ� ���ִ� readlink �̴� */
    printf("readlink : %s %d\n",path,size);
    char npath[1024];

    strcpy(npath,path);
    npath[strlen(path)] = '\0';

    Dentry *newtemp = getFile(npath);

    if(newtemp == NULL)
        return -ENOENT;
    /* symbolic link�� ������ �ִ� data ���Ͽ� �ִ� path�� size��ŭ �״�� �Ѱ��ش� */
    strncpy(buf,fi_de[newtemp->fdnum].data,fi_de[newtemp->fdnum].size);
    buf[fi_de[newtemp->fdnum].size] = '\0';
    return 0;
}

/** Rename a file */
static int fs_rename(const char *oldpath, const char *newpath) {
    /* ���н����� rename�� mv command�� ���Ѵ�.
    ������ ���丮 ������ rename�� �ܼ��� ���ϸ����̸�
    cp�ʹ� �ٸ��� ���Ӱ� ��Ʈ���� �����ϰų� �����ϴ� ������ �Ͼ�� ���� �ƴ�
    �ܼ��� entry�� �Ű��ָ� �ȴ�. */
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
    if(i > 0){ // ��Ʈ ���� ���丮 ����.
        strncpy(npath,newpath,i);
        npath[i] = '\0';
        printf("except name - %s\n",npath);
        newtemp = getFile(npath);
        if(newtemp == NULL) // directory�� ������
            return -ENOENT;
        if((fi_de[newtemp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOTDIR; // ���� ������ directory�� �ƴ�.
        int j;
        for(j = i+1; j<strlen(newpath); j++){
            name[j-i-1] = npath[j];
        }
        name[strlen(newpath)-i-1] = '\0';
        printf("name %s\n",name);

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,name);
    }else{ // ��Ʈ�� ����.
        strcpy(npath,newpath);
        npath[strlen(newpath)] = '\0';

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,npath+1);
        newtemp = &root;


    }

    if(oldtemp->parent == newtemp){ // ������ directory���� �ִٸ� �̸� ���游 �ϸ� ��.
        oldtemp->name = pn;
    }else{ // �ٸ��� directory entry ���� �����������.
        /* ���ϸ� ���� */
        oldtemp->name = pn;
        /* ���� �ִ� �翷 ��Ʈ���� �θ� ���丮�� ���� ó�� */
        if(oldtemp->left_sib != NULL){ // head child�� �ƴ� ���
            oldtemp->left_sib->right_sib = oldtemp->right_sib;
            if(oldtemp->right_sib != NULL)
                oldtemp->right_sib->left_sib = oldtemp->left_sib;
        }else {// head child �� ���
            oldtemp->parent->head_child = oldtemp->right_sib;
            if(oldtemp->right_sib != NULL)
                oldtemp->right_sib->left_sib = oldtemp->left_sib;
        }
        /* �ű�� ������ ���丮�� ��� ���� ���丮�� nlink�� 1 ���ҽ����־����. */
        if((fi_de[oldtemp->fdnum].mode & S_IFDIR) == S_IFDIR){
            fi_de[oldtemp->parent->fdnum].nlink--;
        }
        /* ���Ӱ� directory entry�� �ű�� ���� */
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
    /* hard link�� ��Ÿ���� �� ��쿡�� directory entry�� ����������
    file descriptor�� �������� �ʴ´�. �ٸ�, ����Ű�� file descriptor�� nlink�� 1 ������Ų��. */
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
    if(i > 0){ // ��Ʈ ���� ���丮 ����.
        strncpy(npath,newpath,i);
        npath[i] = '\0';
        printf("except name - %s\n",npath);
        newtemp = getFile(npath);
        if(newtemp == NULL) // directory�� ������
            return -ENOENT;
        if((fi_de[newtemp->fdnum].mode & S_IFDIR) != S_IFDIR)
            return -ENOENT; // ���� ������ directory�� �ƴ�.
        int j;
        for(j = i+1; j<strlen(newpath); j++){
            name[j-i-1] = npath[j];
        }
        name[strlen(newpath)-i-1] = '\0';
        printf("name %s\n",name);

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,name);
    }else{ // ��Ʈ�� ����.
        strcpy(npath,newpath);
        npath[strlen(newpath)] = '\0';

        pn = (char*)malloc(sizeof(char)*(strlen(newpath)+1));

        strcpy(pn,npath+1);
        newtemp = &root;


    }
    /* hard link �̹Ƿ� directory entry�� �����ϰ� file descriptor�� ���� �������ʰ� ���� �ִ°� ����Ŵ. */
    Dentry * child = (Dentry*)malloc(sizeof(Dentry));
    child->name = pn;
    child->fdnum = oldtemp->fdnum;
    printf("oldtemp fdnum : %d %d \n ",oldtemp->fdnum, child->fdnum);
    child->parent = newtemp;
    child->head_child = NULL;
    child->left_sib = NULL;
    child->right_sib = NULL;

    /* �ٸ� file_Cnt �� �÷��ְ�, ������ nlink �� 1 �����ش�. */
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
    /* permission bit�� �ٲ۴�. �� �����ϴ� ���α׷��� �� ������ owner���� Ȯ�����־���Ѵ�. */
    printf("chmod - path : %s \n",path);
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL)
        return -ENOENT;

    struct fuse_context *fc = fuse_get_context();
    if(fc->uid != fi_de[temp->fdnum].uid) // ����� ������ ����� ������ �����ְ� �ƴϸ�
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
    /* ���� ���Ͽ� ���� ���ٱ����� Ȯ���ϰ� �����ش�. */
    printf("open - path : %s \n",path);
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL)
        return -ENOENT;
    if(fi_de[temp->fdnum].mode & S_IFDIR == S_IFDIR)
        return -EISDIR; //  DIRECTORY�̸�.
    /* fdcnt�� ����Ͽ� �����ϰ� �ִ��� ���θ� ��Ÿ���� */
    fdcnt[temp->fdnum]++; // ������ �������Ƿ� count �÷���.

    if((fi->flags & 3) == O_RDONLY){ // ���Ͽ� �����ϴ� �÷��� Ȯ��
        if( (fi_de[temp->fdnum].mode & 0400) == 0)    // owner ������ �����÷��׶� Ȯ��.
            return -EACCES;
    }else if((fi->flags & 3) == O_WRONLY){
        if( (fi_de[temp->fdnum].mode & 0200) == 0)
            return -EACCES;
    }else if((fi->flags & 3) == O_RDWR){
        if( (fi_de[temp->fdnum].mode & 0600) == 0)
            return -EACCES;
    }
    fi->fh = temp->fdnum; // handler�� ���� descriptor�� number�� �Ѱ���.
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
    /* file�� �о���̴� �Լ� �κ�. */
    size_t len;
    (void) fi;
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL) // �����ϴ� �������� Ȯ��
        return -ENOENT;
    // �� ���Ͻý��ۿ��� ������ �� �ִ� �������� ũ��� 1KB �̹Ƿ�, �� ���ϸ� �а� �Ѵ�.
    len = fi_de[temp->fdnum].size;
    printf("len %d offset %d, size %d \n",len,offset,size);
    if (offset < len) {
        if (offset + size > len)
            size = len - offset;
        memcpy(buf, fi_de[temp->fdnum].data + offset, size);
        printf("read text : %s \n",fi_de[temp->fdnum].data);
    } else
        size = 0;
    /* read�� write�� ���� ũ�⸸ŭ�� ��ȯ�Ѵ� */
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
    /* write�� �����ϴ� �Լ��̴� */
    char newpath[1024];
    strcpy(newpath,path);
    newpath[strlen(path)] = '\0';
    Dentry *temp = getFile(newpath);
    if(temp == NULL)
        return -ENOENT;
    size_t len = sizeof(fi_de[temp->fdnum].data);
    printf("len %d offset %d, size %d \n",len,offset,size);
    /* 1KB�� �Ѵ� ������ ������ ���� */
    if(size>len) // ���� ����.
        return -EIO;

    memcpy(fi_de[temp->fdnum].data + offset, buf, size);
    fi_de[temp->fdnum].size = size;
    /* read�� write�� ���� ũ�⸸ŭ�� ��ȯ�Ѵ� */
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
    /* �ѹ��� close�� �Ͼ ������ �߻��ϸ�, open ���� fi �� �Ѱ��� fh�� �ش�Ǵ� fdcnt�� 1 ���ҽ����ش�.
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
    /* �� ���Ͽ� ���� ��� ������ ������ �� �Ҹ��� �Լ��ν�, fdcnt�� 0���� ����� �ش�. */
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
    /* �� directory file ���� ���Ե� ��� ���ϵ��� �о���̴� �Լ��̴�.
    ���� ������ buf�� filler �Լ��� ���� ��� ��ȯ�Ѵ�. */
    printf("readdir - path : %s \n",path);
    (void) offset;
    (void) fi;
    char newpath[1024];
    Dentry *temp;
    struct stat ns;
    memset(&ns,0,sizeof(ns));

    /* filler(buf, name, &stat, 0);
    buf�� name[���ϸ�]�� ���� stat ����ü�� �Ѱ������ν� ls �� ���� ��ɾ ���� ��
    �ܼ��� ���ϸ� ���̴� ���� �ƴ� ���ϸ�� ���Ͽ� �ش�Ǵ� descriptor ������ ��Ÿ����
    �⺻������ .�� ..�� ���н����� �⺻������ ���ԵǾ�� �Ѵ�.
    '.' - �ڱ� �ڽ�  '..' - �θ� */
    if (strcmp(path, "/") == 0){ // path�� root���
        temp = root.head_child;
        setStat(&ns, 1); // root�� �ش�Ǵ� 1�� descriptor file.
        filler(buf, ".", &ns, 0);
        filler(buf, "..", NULL, 0);

    }else{ // root�� �ƴ϶��
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

    /* �ڱ� �ڽ��� ���� ��θ� ����Ѵ�. [temp = head_child ���� null�� �� ������ ������ ���� ��Ʈ���� �̵� ] */
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
    /* ���۽ÿ� root�� ���� �ʱⰪ�� file descriptor �迭 ���� */
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
    /* directory entry�� ���� ��� �޸𸮸� ����Լ��� ���� */
    if(t == NULL)
        return;
    Dentry *temp = t->right_sib;
    while(temp != NULL){// ���� ��� ����
        destroyEntry(temp);
        temp = temp->right_sib;
    }
    destroyEntry(t->head_child); //�ڽ� ����
    free(t);

}

static void fs_destroy(void *data) {
    printf("destroy");
    /* �޸� ���� ������ �̷������. */
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
