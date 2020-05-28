void Pop(Stack* pstack)
{
    if(IsEmpty(pstack))
        exit(1);
    --(pstack->top);
}
