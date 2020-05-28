void DFS(Graph* pgraph)
{
    Stack stack;
    bool* visited=(bool *)malloc(sizeof(bool)*pgraph->num);
    //�� �湮���� ���·� ������.
    for(int i=0;i<pgraph->num;i++)
        visited[i]=false;
    //���� ������.
    InitStack(&stack);

    //�� 0�� �����ϴ°�?
    Push(&stack,0);

    while(!IsEmpty(&stack)){
        GNode* cur;
        int vtx=SPeek(&stack);
        Pop(&stack);

        //�湮�� vertex�� skip ��.
        if(visited[vtx]) continue;
        else{
            //�湮������ ������ ��ȯ����.
            visited[vtx]=true;
            printf("%d",vtx);
        }

        //�湮���� ���� vertex�� �־���.
        cur=pgraph->heads[vtx]->next;
        while(cur!=NULL){
            if(!visited[cur->id])
                Push(&stack,cur->id);
            //�׸��� ���� vertex�� ����Ŵ.
            cur=cur->next;
        }
    }
}
