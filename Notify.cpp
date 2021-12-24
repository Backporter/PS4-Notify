#include <kernel.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
static const char* URL = "https://www.akcpetinsurance.com/res/akc/images/icons/home/home_dog.png";
static bool initialized = false;
int64_t(*sceKernelSendNotificationRequest)(int64_t unk1, char* Buffer, size_t size, int64_t unk2);

void MiraPrint(const char* MessageFMT, ...)
{
	char buffer[1024 * 8];
	va_list args;
	va_start(args, MessageFMT);
	vsprintf(buffer, MessageFMT, args);
	va_end(args);
}

void initialize() {
	int libkernel = sceKernelLoadStartModule("libkernel.sprx", 0, NULL, 0, 0, 0);
	if (libkernel > 0)
	{
		if (sceKernelDlsym(libkernel, "sceKernelSendNotificationRequest", (void **)&sceKernelSendNotificationRequest) != 0)
		{
			MiraPrint("Failed to get address of Symbol");
		}
	}
}

enum NotifyType
{
	NotificationRequest = 0,
	SystemNotification = 1,
	SystemNotificationWithUserId = 2,
	SystemNotificationWithDeviceId = 3,
	SystemNotificationWithDeviceIdRelatedToUser = 4,
	SystemNotificationWithText = 5,
	SystemNotificationWithTextRelatedToUser = 6,
	SystemNotificationWithErrorCode = 7,
	SystemNotificationWithAppId = 8,
	SystemNotificationWithAppName = 9,
	SystemNotificationWithAppInfo = 9,
	SystemNotificationWithAppNameRelatedToUser = 10,
	SystemNotificationWithParams = 11,
	SendSystemNotificationWithUserName = 12,
	SystemNotificationWithUserNameInfo = 13,
	SendAddressingSystemNotification = 14,
	AddressingSystemNotificationWithDeviceId = 15,
	AddressingSystemNotificationWithUserName = 16,
	AddressingSystemNotificationWithUserId = 17,

	UNK_1 = 100,
	TrcCheckNotificationRequest = 101,
	NpDebugNotificationRequest = 102,
	UNK_2 = 102,
};

struct NotifyBuffer
{
	NotifyType Type;
	int ReqId;
	int Priority;
	int MsgId;
	int TargetId;
	int UserId;
	int unk1;
	int unk2;
	int AppId;
	int ErrorNum;
	int unk3;
	char UseIconImageUri;
	char Message[1024];
	char Uri[1024];
	char unkstr[1024];
};


void Notify(char* MessageFMT, ...)
{
	if (!initialized) {
		initialize();
	}

	NotifyBuffer Buffer;

	va_list args;
	va_start(args, MessageFMT);
	vsprintf(Buffer.Message, MessageFMT, args);
	va_end(args);

	Buffer.Type = NotifyType::NotificationRequest;
	Buffer.unk3 = 0;
	Buffer.UseIconImageUri = 1;
	Buffer.TargetId = -1;
	strcpy(Buffer.Uri, URL);

	if (sceKernelSendNotificationRequest == nullptr)
	{
		MiraPrint("sceKernelSendNotificationRequest is a null pointer, we need to avoid calling this or we'll crash");
		MiraPrint(Buffer.Message);
		return;
	}

	sceKernelSendNotificationRequest(0, (char*)&Buffer, 3120, 0);
}