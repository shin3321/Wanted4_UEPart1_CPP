// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "StudentData.h"

UMyGameInstance::UMyGameInstance()
{

}

void UMyGameInstance::Init()
{
	Super::Init();

	// 구조체 객체 생성
	FStudentData RawDataSource(23, TEXT("장세윤"));

	// 파일로 저장하기 위한 경로 생성
	const FString SavePath = FPaths::Combine(FPlatformMisc::ProjectDir(), TEXT("Saved"));

	// 파일 저장 테스트를 위한 구간
	{
		// 파일 이름
		const FString RawDataFileName(TEXT("RawData.bin"));

		// 파일 이름을 포함한 최종 경로 저장 경로 + 파일 이름
		FString RawDataAbsolutePath = FPaths::Combine(SavePath, RawDataFileName);

		// 경로 출력
		UE_LOG(LogTemp, Log, TEXT("저장할 파일의 전체 경로: %s"), *RawDataAbsolutePath);

		FPaths::MakeStandardFilename(RawDataAbsolutePath);

		UE_LOG(LogTemp, Log, TEXT("변경할 파일의 전체 경로: %s"), *RawDataAbsolutePath);

		// 직렬화 사용해서 저장.
		// 파일 열기(쓰기 모드로).
		FArchive* RawFileWriterAr = IFileManager::Get().CreateFileWriter(*RawDataAbsolutePath);

		if (RawFileWriterAr)
		{
			// 파일에 기록
			*RawFileWriterAr << RawDataSource;


			// 파일 닫기
			RawFileWriterAr->Close();

			// 메모리 해제
			delete RawFileWriterAr;
			RawFileWriterAr = nullptr;

			FStudentData RawDataDeserialized;

			FArchive* RawFileReaderAr = IFileManager::Get().CreateFileReader(*RawDataAbsolutePath);
			if (RawFileReaderAr)
			{
				// 구조체로 값 전달 (역직렬화)
				*RawFileReaderAr << RawDataDeserialized;

			// 파일 닫기
				RawFileReaderAr->Close();

				delete RawFileReaderAr;
				RawFileReaderAr = nullptr;

				UE_LOG(LogTemp, Log, TEXT("[RawData] 이름: %s, 순번:  %d"), *RawDataDeserialized.Name, RawDataDeserialized.Order);

			}
		}
	}
}
