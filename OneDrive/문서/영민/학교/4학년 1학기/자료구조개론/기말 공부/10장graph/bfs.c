void BFS(Graph* pgraph)
{
    Queue queue;
    bool* visited=(bool*)malloc(sizeof(bool)* pgraph->num);

    //�����ϱ� ���� �� �湮���� ���� ���·� ����.
    for(int i=0;i<pgraph->num;i++)
        visited[i]=false;

    InitQueue(&queue);

    //initial 0�� ������.
    EnQueue(&queue,0);


    while(!IsQEmpty(&queue)){
        GNode* cur;
        int vtx=QPeek(&queue);
        DeQueue(&queue);

        //�湮�� ��� skip��.
        if(visited[vtx]) continue;
        else{
            visited[vtx]=true;
            printf("%d",vtx);
        }

        //�湮���� ���� ���!
        cur=pgraph->heads[vtx]->next;
        while(cur!=NULL){
            if(!visitd[cur->id])
                EnQueue(&queue,cur->id);
            cur=cur->next;
        }
    }
}
