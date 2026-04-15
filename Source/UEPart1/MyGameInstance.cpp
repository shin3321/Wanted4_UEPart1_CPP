// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "StudentData.h"
#include "Student.h"

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

	// 언리올 오브젝트 생성
	StudentSource = NewObject<UStudent>();
	StudentSource->SetOrder(32);
	StudentSource->SetName(TEXT("로니"));

	{
		const FString& ObjectDataFileName(TEXT("ObjectData.bin"));

		FString ObjectDataAbsolutePath = FPaths::Combine(SavePath, ObjectDataFileName);

		// 경로 값 정리
		FPaths::MakeStandardFilename(ObjectDataAbsolutePath);

		// 메모리를 담을 버퍼
		TArray<uint8>BufferArray;
		FMemoryWriter MemoryWriter(BufferArray);

		// MemoryWriter를 사용해서 언리얼 오브젝트를 직렬화해 값을 기록
		StudentSource->Serialize(MemoryWriter);

		// 파일에 기록
		TUniquePtr<FArchive> FileWriterAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileWriter(*ObjectDataAbsolutePath));

		// 파일이 잘 열렸는지 확인
		if (FileWriterAr)
		{
			// 파일에 기록
			*FileWriterAr << BufferArray;

			// 파일 닫기
			FileWriterAr->Close();
		}

		// 언리얼 오브젝트를 역직렬화
		TArray<uint8> BufferArrayFromFile;
		TUniquePtr<FArchive>FileReaderAr = TUniquePtr<FArchive>(IFileManager::Get().CreateFileReader(*ObjectDataAbsolutePath));

		if (FileReaderAr)
		{
			*FileReaderAr << BufferArrayFromFile;
			FileReaderAr->Close();

			//바이트 배열에 저장된 정보를 메모리 리더를 통해 객체로 복원
			FMemoryReader MemoryReaderAr(BufferArrayFromFile);
			UStudent* StudentDest = NewObject<UStudent>();
			StudentDest->Serialize(MemoryReaderAr);

			// 출력
			UE_LOG(LogTemp, Log, TEXT("[Object Data] 이름: %s, 순번:  %d"), *StudentDest->GetName(), StudentDest->GetOrder());

		}
	}
}
