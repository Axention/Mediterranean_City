// Fill out your copyright notice in the Description page of Project Settings.


#include "CaelumGameInstance.h"

#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

void UCaelumGameInstance::Init()
{
  Super::Init();

  FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UCaelumGameInstance::BeginLoadingScreen);
  FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UCaelumGameInstance::EndLoadingScreen);
}

void UCaelumGameInstance::BeginLoadingScreen(const FString& MapName)
{
  if (!IsRunningDedicatedServer()) {
    const auto LoadingWidget = CreateWidget<UUserWidget>(this, LoadingScreenWidget, TEXT("LoadingScreen"));
    LoadingSWidgetPtr = LoadingWidget->TakeWidget();

    FLoadingScreenAttributes LoadingScreen;
    LoadingScreen.WidgetLoadingScreen = LoadingSWidgetPtr;
    LoadingScreen.bAllowInEarlyStartup = false;
    LoadingScreen.PlaybackType = MT_Normal;
    LoadingScreen.bAllowEngineTick = false;
    LoadingScreen.bWaitForManualStop = false;
    LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
    LoadingScreen.MinimumLoadingScreenDisplayTime = MinimumLoadTime;

    GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
  }
}

void UCaelumGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
  LoadingSWidgetPtr.Reset();
}
