#include <stdio.h> 


void main()

{

	char name[20] = "ȫ�浿";   // �̸�

	char grade = 'B';                 // ���

	int scoreKOR = 80;              // ��������

	int scoreMAT = 75;              // ��������

	int scoreENG = 86;              // ��������

	int scoreSCI = 92;               // ��������



	int scoreTotal = 0;                   // ����

	float scoreAverage = 0.0F;       // ���



	// ���� ���

	scoreTotal = scoreKOR + scoreMAT + scoreENG + scoreSCI;

	// ��� ���

	scoreAverage = scoreTotal / 4;

	printf("%s�� ����\n", name);

	printf("���� ���� : %d\n", scoreKOR);

	printf("���� ���� : %d\n", scoreMAT);

	printf("���� ���� : %d\n", scoreENG);

	printf("���� ���� : %d\n", scoreSCI);

	printf("���� : %d\n��� : %f\n", scoreTotal, scoreAverage);


}
