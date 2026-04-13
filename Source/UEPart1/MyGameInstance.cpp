// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Student.h"
#include "Teacher.h"
#include "Staff.h"
#include "Card.h"
#include "CourseInfo.h"
#include "Algo/Accumulate.h"

UMyGameInstance::UMyGameInstance()
{
	// 기본 값은 CDO라는 특별한 템플릿 객체에 저장됨
	SchoolName = TEXT("기본 학교");
}

void UMyGameInstance::Init()
{
	Super::Init();

	// TArray
	const int32 ArrayNum = 10;
	TArray<int32> IntArray;

	for (int32 i = 0; i < ArrayNum; ++i)
	{
		IntArray.Add(i);
	}

	// 짝수 제거
	IntArray.RemoveAll(
		// [] 캡처 (외부 내용을 람다 안에서 사용할 때 활용) 
		// () 파라미터
		// -> 반환형
		// {} 본문
		[](int32 Val) -> bool
		{
			return Val % 2 == 0;
		}
	);

	// 짝수 삽입
	IntArray += {2, 4, 6, 8, 10 };

	// 비교
	TArray<int32> IntArrayCompare;
	int32 CArray[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 10 };

	IntArrayCompare.AddUninitialized(ArrayNum);

	// C 스타일 배열을 TArray에 메모리 복사
	FMemory::Memcpy(IntArrayCompare.GetData(), CArray, sizeof(int32) * ArrayNum);


	/// 어서트 - 크래시를 발생시키지 않고 출력 로그 창에 오류 메시지 출력
	ensureAlways(IntArray == IntArrayCompare);

	// 합계
	int32 Sum = 0;
	for (const int32& Int32Num : IntArray)
	{
		Sum += Int32Num;
	}

	// 알고리즘 활용 - 합계 구하기
	int32 SumByAlgo = Algo::Accumulate(IntArray, 0);
	ensureAlways(Sum == SumByAlgo);
}
