void BFS(Graph* pgraph)
{
    Queue queue;
    bool* visited=(bool*)malloc(sizeof(bool)* pgraph->num);

    //시작하기 전에 다 방문하지 않은 상태로 세팅.
    for(int i=0;i<pgraph->num;i++)
        visited[i]=false;

    InitQueue(&queue);

    //initial 0을 삽입함.
    EnQueue(&queue,0);


    while(!IsQEmpty(&queue)){
        GNode* cur;
        int vtx=QPeek(&queue);
        DeQueue(&queue);

        //방문한 경우 skip함.
        if(visited[vtx]) continue;
        else{
            visited[vtx]=true;
            printf("%d",vtx);
        }

        //방문하지 않은 경우!
        cur=pgraph->heads[vtx]->next;
        while(cur!=NULL){
            if(!visitd[cur->id])
                EnQueue(&queue,cur->id);
            cur=cur->next;
        }
    }
}
