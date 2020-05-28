#include <stdio.h> 


void main()

{

	char name[20] = "홍길동";   // 이름

	char grade = 'B';                 // 등급

	int scoreKOR = 80;              // 국어점수

	int scoreMAT = 75;              // 수학점수

	int scoreENG = 86;              // 영어점수

	int scoreSCI = 92;               // 과학점수



	int scoreTotal = 0;                   // 총점

	float scoreAverage = 0.0F;       // 평균



	// 총점 계산

	scoreTotal = scoreKOR + scoreMAT + scoreENG + scoreSCI;

	// 평균 계산

	scoreAverage = scoreTotal / 4;

	printf("%s의 성적\n", name);

	printf("국어 점수 : %d\n", scoreKOR);

	printf("수학 점수 : %d\n", scoreMAT);

	printf("영어 점수 : %d\n", scoreENG);

	printf("과학 점수 : %d\n", scoreSCI);

	printf("총점 : %d\n평균 : %f\n", scoreTotal, scoreAverage);


}
