void DFS(Graph* pgraph)
{
    Stack stack;
    bool* visited=(bool *)malloc(sizeof(bool)*pgraph->num);
    //다 방문안한 상태로 세팅함.
    for(int i=0;i<pgraph->num;i++)
        visited[i]=false;
    //스택 생성함.
    InitStack(&stack);

    //왜 0을 삽입하는가?
    Push(&stack,0);

    while(!IsEmpty(&stack)){
        GNode* cur;
        int vtx=SPeek(&stack);
        Pop(&stack);

        //방문한 vertex는 skip 함.
        if(visited[vtx]) continue;
        else{
            //방문했으면 참으로 교환해줌.
            visited[vtx]=true;
            printf("%d",vtx);
        }

        //방문하지 않은 vertex를 넣어줌.
        cur=pgraph->heads[vtx]->next;
        while(cur!=NULL){
            if(!visited[cur->id])
                Push(&stack,cur->id);
            //그리고 다음 vertex를 가리킴.
            cur=cur->next;
        }
    }
}
