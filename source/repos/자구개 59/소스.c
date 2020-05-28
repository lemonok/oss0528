int i, n, *list;
printf("enter the number ofs to generate: ");
scanf("%d", &n);
if (n < 1) {
	fprinf(stderr, "Improper value of n\n");
	exit(EXIT_FAILURE);
}