#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<string.h>
#include<time.h>
#include<Windows.h>

typedef struct
{
	int hesapId;
	int hesapBakiyesi;
	int hesapSayisi;
}hesap;
typedef struct
{
	int musteriId;
	char musteriAdi[51];
	char musteriPin[5];
	int musteriTipi;
	int gunlukCekimLimiti;
	int gunKontrol;
	hesap musteriHesabi;
}musteri;

void customerDailyCheck(int type, int limDaily);
int currentTime();
int numberScanf(int i);
void pinScanf(char *pin);
void fileCreater(char *fileName);
void acccountCheck(musteri *returnOwner);
int selector(int maxSelectionValue);
void backToAccountTransactionsChoice(musteri *sendOwner);
int idControl(int *idCont, musteri *owner, int controlId);
void tempReplacer(FILE *fp, FILE *fpTemp);


void accountTransactions(musteri *sendOwner);
void addNewCustomer(musteri *sendOwner);
void newCustomerIdCreater(int *customerType, int *newId);
int newCustomerAccountIdCreater();
void createAnAccount(musteri *sendOwner);
void idControlChange(int controlId, int newAccountId, FILE *fp);
void withdraw(musteri *sendOwner);
void deposit(musteri *sendOwner);
void transfer(musteri *sendOwner);
void report(musteri *sendOwner);
void deleteAcc(musteri *sendOwner);
void record(musteri *sendOwner);

int main() {
	char fileName[50] = { "bireysel.txt" };
	fileCreater(fileName);
	strcpy(fileName, "ticari.txt");
	fileCreater(fileName);
	musteri owner;
	customerDailyCheck(0,0);
	acccountCheck(&owner);
	accountTransactions(&owner);
	return 0;
}

void customerDailyCheck(int type,int limDaily) {
	int i;
	FILE *fp, *fptemp;
	musteri owner;
	char chr;
	for (i = 0; i < 2; i++)
	{
		if (i == 0) {
			fp = fopen("bireysel.txt", "r");
			fptemp = fopen("temp.txt", "w");
		}
		else
		{
			fp = fopen("ticari.txt", "r");
			fptemp = fopen("temp.txt", "w");
		}
		if (fp != NULL)
		{
			chr = fgetc(fp);
			if (feof(fp))
			{
				fclose(fp);
				fclose(fptemp);
				return;
			}
			else
			{
				while (!feof(fp))
				{
					fputc(chr, fptemp);
					if (chr == '*')
					{
						fscanf(fp, "%d", &owner.musteriId);
						fscanf(fp, "%d", &owner.musteriHesabi.hesapSayisi);
						fscanf(fp, "%s", owner.musteriPin);
						fscanf(fp, "%s", owner.musteriAdi);
						fscanf(fp, "%d", &owner.gunlukCekimLimiti);
						fscanf(fp, "%d", &owner.gunKontrol);
						if (type == 0)
						{
							if (owner.gunKontrol != currentTime())
							{
								owner.gunKontrol = currentTime();
								if ((owner.musteriId / 100000) == 1)
									owner.gunlukCekimLimiti = 750;
								else
									owner.gunlukCekimLimiti = 1500;
							}
						}
						else
						{
							if (type==owner.musteriId)
							{
								owner.gunlukCekimLimiti = limDaily;
							}
						}				
						fprintf(fptemp, "\n%d", owner.musteriId);
						fprintf(fptemp, "\n%d", owner.musteriHesabi.hesapSayisi);
						fprintf(fptemp, "\n%s", owner.musteriPin);
						fprintf(fptemp, "\n%s", owner.musteriAdi);
						fprintf(fptemp, "\n%d ", owner.gunlukCekimLimiti);
						fprintf(fptemp, "%d", owner.gunKontrol);
					}
					chr = fgetc(fp);

				}
				fclose(fp);
				fclose(fptemp);
				if ((fptemp = fopen("temp.txt", "r")) != NULL)
				{
					if (i == 0)
					{
						fp = fopen("bireysel.txt", "w");
					}
					else
					{
						fp = fopen("ticari.txt", "w");
					}
					tempReplacer(fp, fptemp);
				}


			}
		}
	}
	remove("temp.txt");
}

int currentTime() {
	struct tm *lt;
	time_t t;
	time(&t);
	lt = localtime(&t);
	int r;
	r = ((lt->tm_year + 1900)*(10000)) + ((lt->tm_mon + 1)*(100)) + (lt->tm_mday);
	return r;
}

int numberScanf(int iMax) {
	char chr;
	int num = 0, i = 0;
	while (1)
	{
		if ((iMax > 0) && (i == iMax))
		{
			break;
		}
		chr = getch();
		if ((chr == 8) && (i > 0)) {
			putchar(chr);
			putchar(' ');
			putchar(chr);
			i--;
			num = num / 10;
		}
		else if (((chr <= 57) && (chr >= 48)))
		{
			putchar(chr);
			i++;
			num = (num * 10) + (chr - 48);
		}
		else if (chr == 13)
		{
			break;
		}

	}
	putchar('\n');
	return num;

}

void pinScanf(char *pin) {
	char chr;
	int i;
	for (i = 0; i < 4; i)
	{
		chr = getch();
		if ((chr == 8) && (i > 0)) {
			putchar(chr);
			putchar(' ');
			putchar(chr);
			i--;
		}
		else if (((chr <= 57) && (chr >= 48)))
		{
			putchar('*');
			*(pin + i) = chr;
			i++;
		}
	}

	*(pin + i) = '\0';
}

void fileCreater(char *fileName) {

	FILE *fp;
	fp = fopen(fileName, "a");
	fclose(fp);
}

void acccountCheck(musteri *returnOwner) {
	int idCont = 0, count = 0, pinCont, fileCont = 0;
	int controlId = 0;
	char controlPin[5];
	musteri owner;
	do
	{
		idCont = 0;
		printf("Musteri id'nizi giriniz: ");
		controlId = numberScanf(6);
		owner.musteriTipi = controlId / 100000;
		idControl(&idCont, &owner, controlId);
		if (idCont == 1)
		{
			printf("Girdiginiz id ile ilgili bir kayit yoktur\n");
		}
		((idCont == 1) || (idCont == 2)) ? controlId = 0 : 1;
	} while ((idCont == 1) || (idCont == 2));

	do
	{
		printf("Lutfen pininizi giriniz: ");
		pinScanf(controlPin);

		if (!(strcmp(controlPin, owner.musteriPin)))
		{
			printf("\nGiris bilgileriniz dogrulanmistir");
			pinCont = 0;
			count = 0;
			getch();
		}
		else
		{
			printf("\nGecersiz sifre tekrar deneyiniz\n");
			pinCont = 1;
			count++;
			if (count == 3)
			{
				printf("\nUc kere gecersiz sifre girdiniz isleminiz durduruluyor\n");
				getch();
				exit(1);
			}
		}

	} while (pinCont);
	if (owner.musteriTipi == 1 || owner.musteriTipi == 2)
	{
		system("cls");
		printf("Hosgeldiniz %s\n", owner.musteriAdi);
	}
	else
	{
		system("cls");
	}
	*(returnOwner) = owner;
}

int idControl(int *idCont, musteri *owner, int controlId) {
	FILE *fp;
	if (owner->musteriTipi == 1)
	{
		fp = fopen("bireysel.txt", "r");
	}
	else if (owner->musteriTipi == 2)
	{
		fp = fopen("ticari.txt", "r");
	}
	else if (controlId == 654321)
	{
		*idCont = 3;
		owner->musteriId = 654321;
		strcpy(owner->musteriPin, "1234");
		return 0;
	}
	else
	{
		*idCont = 1;
		owner->musteriId = 0;
		strcpy(owner->musteriPin, "");
		strcpy(owner->musteriAdi, "");
		return 0;
	}
	char chr;
	int i;
	if (fp != NULL)
	{
		chr = fgetc(fp);
		if (feof(fp))
		{
			fclose(fp);
			printf("Dosyada musteri yok banka idsi ile musteri ekleme yapiniz\n");
			*idCont = 2;
			return 0;
		}
		else
		{
			while (!feof(fp)) {

				if (chr == '*')
				{
					fscanf(fp, "%d", &owner->musteriId);
					fscanf(fp, "%d", &owner->musteriHesabi.hesapSayisi);
					fscanf(fp, "%s", owner->musteriPin);
					fscanf(fp, "%s", owner->musteriAdi);
					fscanf(fp, "%d", &owner->gunlukCekimLimiti);
					fscanf(fp, "%d", &owner->gunKontrol);
					for (i = 0; i < strlen(owner->musteriAdi); i++)
					{
						if (*(owner->musteriAdi + i) == '.')
						{
							*(owner->musteriAdi + i) = ' ';
						}
					}
					if (owner->musteriId == controlId)
					{
						*idCont = 0;
						fclose(fp);
						return 1;
					}
					else
					{
						*idCont = 1;
					}
				}
				chr = fgetc(fp);
			}
			fclose(fp);
		}

	}
	return 0;
}

void accountTransactions(musteri *sendOwner) {
	int choice;
	if (sendOwner->musteriTipi == 6)
	{
		printf("1.Yeni musteri ekleme\n");
		printf("2.Kayitli musteriye yeni hesap actirma\n");
		printf("3.Kayitli musterinin hesabini silme\n");
		printf("4.Banka gelir gider raporu\n");
		printf("Seciminizi giriniz: ");
		choice = selector(4);
		if (choice == 1)
		{
			addNewCustomer(sendOwner);
		}
		if (choice == 2)
		{
			createAnAccount(sendOwner);
		}
		if (choice == 3)
		{
			deleteAcc(sendOwner);
		}
		if (choice == 4)
		{
			report(sendOwner);
		}
	}
	else
	{
		printf("1.Para cekme\n");
		printf("2.Para yatirma\n");
		printf("3.Baska hesaba havale\n");
		printf("4.Hesap dekontu\n");
		printf("Seciminizi giriniz: ");
		choice = selector(4);
		if (choice == 1)
		{
			withdraw(sendOwner);
		}
		if (choice == 2)
		{
			deposit(sendOwner);
		}
		if (choice == 3)
		{
			transfer(sendOwner);
		}
		if (choice == 4)
		{
			record(sendOwner);
		}
	}
}

int selector(int maxSelectionValue) {
	int choice, minSelectionValue = 1;
	if (maxSelectionValue > 9)
	{
		maxSelectionValue = 9;
	}
	choice = (getch()) - 48;
	(choice < minSelectionValue || choice>maxSelectionValue) ? 1 : printf("%d", choice);
	while (choice < minSelectionValue || choice>maxSelectionValue)
	{
		printf("\nSeciminizi yanlis girdiniz tekrar seciminizi giriniz: ");
		choice = (getch()) - 48;
		(choice < minSelectionValue || choice>maxSelectionValue) ? 1 : printf("%d", choice);
	}
	putchar('\n');
	return choice;

}

void backToAccountTransactionsChoice(musteri *sendOwner) {
	int choice;
	printf("\nBaska islem yapmak istiyor musunuz(1.Evet 2.Hayir):");
	choice = selector(2);
	if (choice == 1)
	{
		putchar('\n');
		accountTransactions(sendOwner);
	}
	else
	{
		printf("Hoscakalin");
		Sleep(1000);
		exit(1);

	}
}

void addNewCustomer(musteri *sendOwner) {
	int i = 0, choice;
	srand(time(0));
	musteri newCustomer;
	FILE *fp;
	printf("\n1.Bireysel\n2.Ticari\n");
	printf("Musterinin turunu seciniz: ");
	newCustomer.musteriTipi = selector(2);
	do
	{
		printf("Musterinin adini soyadini giriniz (en fazla bosluklar dahil 50 harf): \n");
		i = 0;
		char chr;
		for (i = 0; i < 50; i)
		{
			chr = getch();
			if ((chr == 8) && (i > 0)) {
				putchar(chr);
				putchar(' ');
				putchar(chr);
				i--;
			}
			else if ((chr != 13) && (chr != 8))
			{
				putchar(chr);

				if (!(((chr <= 90) && (chr >= 65)) || ((chr <= 122) && (chr >= 97))))
				{
					chr = ' ';
				}
				*(newCustomer.musteriAdi + i) = chr;
				i++;
			}
			else if (chr == 13)
			{
				break;
			}
		}
		putchar('\n');
		*(newCustomer.musteriAdi + i) = '\0';
		printf("%s ismini onayliyor musunuz(1.Evet 2.Hayir):", newCustomer.musteriAdi);
		choice = selector(2) - 1;
	} while (choice);
	for (i = 0; i < 4; i++)
	{
		if (i == 0)
		{
			*(newCustomer.musteriPin + i) = 48 + ((rand() % 9) + 1);
		}
		else
		{
			*(newCustomer.musteriPin + i) = 48 + (char)(rand() % 10);
		}
	}
	*(newCustomer.musteriPin + i) = '\0';
	newCustomerIdCreater(&newCustomer.musteriTipi, &newCustomer.musteriId);
	newCustomer.musteriHesabi.hesapId = newCustomerAccountIdCreater();
	newCustomer.gunKontrol = currentTime();
	char id[7], dosya[25] = "hesaplar/";
	itoa(newCustomer.musteriId, id, 10);
	strcat(dosya, id);
	strcat(dosya, ".txt");
	fp = fopen(dosya, "w");
	if (newCustomer.musteriTipi == 1)
	{
		fprintf(fp, "500");
		fclose(fp);
		fp = fopen("bireysel.txt", "a");
		newCustomer.gunlukCekimLimiti = 750;

	}
	else
	{
		fprintf(fp, "1000");
		fclose(fp);
		fp = fopen("ticari.txt", "a");
		newCustomer.gunlukCekimLimiti = 1500;
	}
	for (i = 0; i < strlen(newCustomer.musteriAdi); i++)
	{
		if (*(newCustomer.musteriAdi + i) == ' ')
		{
			*(newCustomer.musteriAdi + i) = '.';
		}
	}
	if (fp != NULL)
	{
		fprintf(fp, "*\n");
		fprintf(fp, "%d\n", newCustomer.musteriId);
		fprintf(fp, "1\n");
		fprintf(fp, "%s\n", newCustomer.musteriPin);
		fprintf(fp, "%s\n", newCustomer.musteriAdi);
		fprintf(fp, "%d ", newCustomer.gunlukCekimLimiti);
		fprintf(fp, "%d\n", newCustomer.gunKontrol);
		fprintf(fp, "_\n");
		fprintf(fp, "%d\n", newCustomer.musteriHesabi.hesapId);
		fclose(fp);

	}
	strcpy(dosya, "hesaplar/");
	itoa(newCustomer.musteriId, id, 10);
	strcat(dosya, id);
	itoa(newCustomer.musteriHesabi.hesapId, id, 10);
	strcat(dosya, id);
	strcat(dosya, ".txt");
	fp = fopen(dosya, "w");
	fprintf(fp, "0\n");
	fclose(fp);
	putchar('\n');
	printf("Yeni eklenen musterinin bilgileri not etmeyi unutmayin...\n");
	printf("Musterinin idsi: %d\n", newCustomer.musteriId);
	printf("Musterinin pini: %s\n", newCustomer.musteriPin);
	printf("Musterinin hesap idsi: %d\n", newCustomer.musteriHesabi.hesapId);
	Sleep(4000);
	backToAccountTransactionsChoice(sendOwner);
}

void newCustomerIdCreater(int *customerType, int *newId) {
	int choice, controlId;
	FILE *fp;
	char chr;
	do
	{
		choice = 0;
		*newId = rand() % 100000;
		*newId += ((*customerType) * 100000);
		if (*customerType == 1)
		{
			fp = fopen("bireysel.txt", "r");
			if (fp != NULL)
			{
				chr = fgetc(fp);
				while (!feof(fp)) {
					if (chr == '*')
					{
						fscanf(fp, "%d", &controlId);
						if (controlId == *newId)
						{
							choice = 1;
							fclose(fp);
							break;
						}

					}
					chr = fgetc(fp);
				}
				fclose(fp);
			}
		}
		else
		{
			fp = fopen("ticari.txt", "r");
			if (fp != NULL)
			{
				chr = fgetc(fp);
				while (!feof(fp)) {
					if (chr == '*')
					{
						fscanf(fp, "%d", &controlId);
						if (controlId == *newId)
						{
							choice = 1;
							fclose(fp);
							break;
						}
					}
					chr = fgetc(fp);
				}
				fclose(fp);
			}
		}
	} while (choice);
}

int newCustomerAccountIdCreater() {
	int control, newAccountId, controlId, i, hesapSayi;
	FILE *fp;
	char chr;
	do
	{
		control = 0;
		newAccountId = rand() % 100000;
		if (newAccountId < 10000)
		{
			newAccountId += 10000;
		}
		fp = fopen("bireysel.txt", "r");
		if (fp != NULL)
		{
			chr = fgetc(fp);
			while (!feof(fp)) {
				if (chr == '*')
				{
					fscanf(fp, "%d", &hesapSayi);
					fscanf(fp, "%d", &hesapSayi);
				}
				if (chr == '_')
				{
					for (i = 0; i < hesapSayi; i++)
					{
						fscanf(fp, "%d", &controlId);
						if (controlId == newAccountId)
						{
							control = 1;
							fclose(fp);
							break;
						}
					}


				}
				if (control==1)
				{
					break;
				}
				chr = fgetc(fp);
			}
			fclose(fp);
		}
		fp = fopen("ticari.txt", "r");
		if (fp != NULL)
		{
			chr = fgetc(fp);
			while (!feof(fp)) {
				if (chr == '*')
				{
					fscanf(fp, "%d", &hesapSayi);
					fscanf(fp, "%d", &hesapSayi);
				}
				if (chr == '_')
				{
					for (i = 0; i < hesapSayi; i++)
					{
						fscanf(fp, "%d", &controlId);
						if (controlId == newAccountId)
						{
							control = 1;
							fclose(fp);
							break;
						}
					}

				}
				if (control == 1)
				{
					break;
				}
				chr = fgetc(fp);
			}
			fclose(fp);
		}
	} while (control);
	return newAccountId;
}

void createAnAccount(musteri *sendOwner) {
	srand(time(0));
	int  count = 0, fileCont = 0;
	int controlId = 0, idCont;
	musteri owner;
	FILE *fp;
	printf("\nMusteri id'nizi giriniz: ");
	controlId = numberScanf(6);
	owner.musteriTipi = controlId / 100000;
	if (idControl(&idCont, &owner, controlId))
	{
		owner.musteriHesabi.hesapId = newCustomerAccountIdCreater();
		if (owner.musteriTipi == 1)
		{
			fp = fopen("bireysel.txt", "r");
			idControlChange(controlId, owner.musteriHesabi.hesapId, fp);
		}
		else if (owner.musteriTipi == 2)
		{
			fp = fopen("ticari.txt", "r");
			idControlChange(controlId, owner.musteriHesabi.hesapId, fp);
		}
		FILE *fpTemp = fopen("temp.txt", "r");
		char dosya[25], id[7];
		if (fpTemp != NULL)
		{
			if (owner.musteriTipi == 1)
			{

				fp = fopen("bireysel.txt", "w");
				tempReplacer(fp, fpTemp);
			}
			else if (owner.musteriTipi == 2)
			{

				fp = fopen("ticari.txt", "w");
				tempReplacer(fp, fpTemp);
			}
			strcpy(dosya, "hesaplar/");
			itoa(controlId, id, 10);
			strcat(dosya, id);
			itoa(owner.musteriHesabi.hesapId, id, 10);
			strcat(dosya, id);
			strcat(dosya, ".txt");
			fp = fopen(dosya, "w");
			fprintf(fp, "0\n");
			fclose(fp);

		}
	}
	else
	{
		printf("%d idli musteri yok\n", controlId);
	}
	backToAccountTransactionsChoice(sendOwner);
}

void idControlChange(int controlId, int newAccountId, FILE *fp) {
	FILE *fpTemp = fopen("temp.txt", "w");
	char chr;
	int ownerId, accountCount, idCont = 0;
	if (fp != NULL)
	{
		chr = fgetc(fp);
		while (!feof(fp)) {

			fputc(chr, fpTemp);
			if (chr == '*')
			{
				fscanf(fp, "%d", &ownerId);
				fprintf(fpTemp, "\n%d", ownerId);
				fscanf(fp, "%d", &accountCount);
				if (ownerId == controlId)
				{
					++accountCount;
					idCont = 1;
				}
				else
				{
					idCont = 0;
				}

				fprintf(fpTemp, "\n%d", accountCount);
			}
			chr = fgetc(fp);
			if (((chr == '*') || feof(fp)) && (idCont))
			{
				fprintf(fpTemp, "%d\n", newAccountId);
			}
		}
		fclose(fp);
		fclose(fpTemp);
	}
	printf("\nNot etmeyi unutmayin...\n%d idli musterinin yeni hesabinin idsi: %d", controlId, newAccountId);
	Sleep(4000);
}

void tempReplacer(FILE *fp, FILE *fpTemp)
{
	char chr;
	while (!feof(fpTemp)) {
		chr = fgetc(fpTemp);
		if (feof(fpTemp))
		{
			break;
		}
		fputc(chr, fp);
	}
	fclose(fp);
	fclose(fpTemp);
	fpTemp = fopen("temp.txt", "w");
	fclose(fpTemp);
	remove("temp.txt");
}

void withdraw(musteri *sendOwner){
	int amount,ekBakiye,surplus=0,choice=0;
	char fileName[25] = "hesaplar/",fileNameEx[20]="hesaplar/", id[7];
	FILE *fp, *fpEx;
	printf("Hesap Id giriniz: ");
	sendOwner->musteriHesabi.hesapId = numberScanf(5);
	itoa(sendOwner->musteriId, id, 10);
	strcat(fileName,id);
	itoa(sendOwner->musteriHesabi.hesapId, id, 10);
	strcat(fileName, id);
	strcat(fileName, ".txt");
	fp = fopen(fileName,"r");
	itoa(sendOwner->musteriId, id, 10);
	strcat(fileNameEx, id);
	strcat(fileNameEx, ".txt");
	fpEx = fopen(fileNameEx,"r");
	if ((fp!=0)&&(fpEx!=0))
	{
		fscanf(fp, "%d", &sendOwner->musteriHesabi.hesapBakiyesi);
		printf("Bakiyeniz:%d TL\n", sendOwner->musteriHesabi.hesapBakiyesi);
		printf("Cekeceginiz tutari giriniz (bugun en fazla %d Tl): ",sendOwner->gunlukCekimLimiti);
		amount = numberScanf(0);
		if(amount==0){
			printf("Cekilecek para tutari 0 TL olamaz isleminiz iptal edilmistir...\n");
			fclose(fp);
			fclose(fpEx);
			backToAccountTransactionsChoice(sendOwner);
		}
		if ((sendOwner->gunlukCekimLimiti - amount) >= 0)
		{
			sendOwner->gunlukCekimLimiti -= amount;
			fscanf(fpEx,"%d",&ekBakiye);
			if ((sendOwner->musteriHesabi.hesapBakiyesi - amount) < 0)
			{
				surplus = (sendOwner->musteriHesabi.hesapBakiyesi - amount)*-1;
				sendOwner->musteriHesabi.hesapBakiyesi=0;
				if ((ekBakiye-surplus)>=0)
				{
					printf("Hesabin bakiyesi yetmiyor %d TL icin ek hesabi kullanalim mi\nEk hesap bakiyesi %d Tl (1.Evet/2.Hayir): ",surplus,ekBakiye);
					choice = selector(2);
					if (choice-1)
					{
						printf("Isleminiz iptal edilmistir...\n");
						fclose(fp);
						fclose(fpEx);
						sendOwner->gunlukCekimLimiti += amount;
						backToAccountTransactionsChoice(sendOwner);
					}
					else
					{
						ekBakiye -= surplus;
					}
				}
				else
				{
					printf("Hesabinizin ve ek hesabinizin bakiyesi yetersiz oldugundan isleminiz iptal edilmistir...\n");
					fclose(fp);
					fclose(fpEx);
					sendOwner->gunlukCekimLimiti += amount;
					backToAccountTransactionsChoice(sendOwner);
				}
			}
			else
			{
				sendOwner->musteriHesabi.hesapBakiyesi -= amount;
			}
			fclose(fpEx);
			fpEx = fopen(fileNameEx,"w");
			fprintf(fpEx,"%d",ekBakiye);
			fclose(fpEx);
			char chr;
			FILE *fpTemp = fopen("temp.txt", "w");
			fprintf(fpTemp,"%d", sendOwner->musteriHesabi.hesapBakiyesi);
			while (!feof(fp)) {
				chr = fgetc(fp);
				if (feof(fp)) 
				{
					break;
				}
				fputc(chr, fpTemp);
			}
			int date=currentTime(),day,month,year;
			day = date % 100;
			date /= 100;
			if (day<10)
			{
				fprintf(fpTemp, "0");
			}
			fprintf(fpTemp, "%d.", day);
			month = date % 100;
			if (month<10)
			{
				fprintf(fpTemp, "0");
			}
			fprintf(fpTemp, "%d.", month);
			year = date / 100;
			fprintf(fpTemp,"%d - Para cekildi\t%d TL",year,amount);
			if (choice==1)
			{
				fprintf(fpTemp, " - (%d TL ek hesaptan cekildi)", surplus);
			}
			fputc('\n',fpTemp);
			fclose(fpTemp);
			fclose(fp);
			fp = fopen(fileName,"w");
			fopen("temp.txt","r");
			tempReplacer(fp,fpTemp);
			printf("Isleminiz basari ile gerceklesti...\n");
			int bankIn, bankOut, bankTotal;
			fp = fopen("rapor.txt", "r");
			fscanf(fp, "%d", &bankIn);
			fscanf(fp, "%d", &bankOut);
			fscanf(fp, "%d", &bankTotal);
			bankOut += amount;
			bankTotal = bankIn - bankOut;
			fp = fopen("rapor.txt", "w");
			fprintf(fp, "%d\n", bankIn);
			fprintf(fp, "%d\n", bankOut);
			fprintf(fp, "%d\n", bankTotal);
			fclose(fp);
			customerDailyCheck(sendOwner->musteriId, sendOwner->gunlukCekimLimiti);
			backToAccountTransactionsChoice(sendOwner);
		}
	}
	else
	{
		printf("Girmis oldugunuz %d idli bir hesabiniz yoktur...\n", sendOwner->musteriHesabi.hesapId);
		sendOwner->musteriHesabi.hesapId = 0;
		backToAccountTransactionsChoice(sendOwner);
	}
}

void deposit(musteri *sendOwner){
	{
		int amount, ekBakiye, surplus = 0, choice = 0;
		char fileName[25] = "hesaplar/", fileNameEx[20] = "hesaplar/", id[7];
		FILE *fp, *fpEx;
		printf("Hesap Id giriniz: ");
		sendOwner->musteriHesabi.hesapId = numberScanf(5);
		itoa(sendOwner->musteriId, id, 10);
		strcat(fileName, id);
		itoa(sendOwner->musteriHesabi.hesapId, id, 10);
		strcat(fileName, id);
		strcat(fileName, ".txt");
		fp = fopen(fileName, "r");
		itoa(sendOwner->musteriId, id, 10);
		strcat(fileNameEx, id);
		strcat(fileNameEx, ".txt");
		fpEx = fopen(fileNameEx, "r");
		if ((fp != 0) && (fpEx != 0))
		{
			fscanf(fpEx, "%d", &ekBakiye);
			fscanf(fp, "%d", &sendOwner->musteriHesabi.hesapBakiyesi);
			printf("Bakiyeniz:%d TL\n",sendOwner->musteriHesabi.hesapBakiyesi);
			int debt=500;
			debt = (debt)*(sendOwner->musteriTipi);
			if ((ekBakiye-debt)<0)
			{
				printf("Yatiracaginiz tutari giriniz (ek hesabinizda %d TL borc vardir): ", ekBakiye - debt);
			}
			else
			{
				printf("Yatiracaginiz tutari giriniz: ");
			}
			amount = numberScanf(0);
			if(amount==0){
			printf("Yatirilacak para tutari 0 TL olamaz isleminiz iptal edilmistir...\n");
			fclose(fp);
			fclose(fpEx);
			backToAccountTransactionsChoice(sendOwner);
		}
			if ((ekBakiye - debt) < 0)
			{
				choice = 1;
				surplus = amount + (ekBakiye - debt);
				if (surplus<0)
				{
					surplus = 0;
				}
				ekBakiye += amount - surplus;
			}
			else
			{
				surplus = amount;
			}
			
			sendOwner->musteriHesabi.hesapBakiyesi += surplus;
			fclose(fpEx);
			fpEx = fopen(fileNameEx, "w");
			fprintf(fpEx, "%d", ekBakiye);
			fclose(fpEx);
			char chr;
			FILE *fpTemp = fopen("temp.txt", "w");
			fprintf(fpTemp, "%d", sendOwner->musteriHesabi.hesapBakiyesi);
			while (!feof(fp)) {
				chr = fgetc(fp);
				if (feof(fp))
				{
					break;
				}
				fputc(chr, fpTemp);
			}
			int date = currentTime(), day, month, year;
			day = date % 100;
			date /= 100;
			if (day < 10)
			{
				fprintf(fpTemp, "0");
			}
			fprintf(fpTemp, "%d.", day);
			month = date % 100;
			if (month < 10)
			{
				fprintf(fpTemp, "0");
			}
			fprintf(fpTemp, "%d.", month);
			year = date / 100;
			fprintf(fpTemp, "%d - Para yatirildi\t%d TL", year, amount);
			if (choice == 1)
			{
				fprintf(fpTemp, " - (%d TL ek hesaba yatirildi)", amount - surplus);
			}
			fputc('\n', fpTemp);
			fclose(fpTemp);
			fclose(fp);
			fp = fopen(fileName, "w");
			fopen("temp.txt", "r");
			tempReplacer(fp, fpTemp);
			printf("Isleminiz basari ile gerceklesti...\n");
			int bankIn, bankOut, bankTotal;
			fp = fopen("rapor.txt", "r");
			fscanf(fp, "%d", &bankIn);
			fscanf(fp, "%d", &bankOut);
			fscanf(fp, "%d", &bankTotal);
			bankIn += amount;
			bankTotal = bankIn - bankOut;
			fp = fopen("rapor.txt", "w");
			fprintf(fp, "%d\n", bankIn);
			fprintf(fp, "%d\n", bankOut);
			fprintf(fp, "%d\n", bankTotal);
			fclose(fp);
			backToAccountTransactionsChoice(sendOwner);
		}
		else
		{
			printf("Girmis oldugunuz %d idli bir hesabiniz yoktur...\n", sendOwner->musteriHesabi.hesapId);
			sendOwner->musteriHesabi.hesapId = 0;
			backToAccountTransactionsChoice(sendOwner);
		}
	}
}

void transfer(musteri *sendOwner) {
	int  controlId, accountId, i, j, idi, cont = 1, day, month, year;
	musteri recipient;
	printf("Gonderilecek hesabin id'sini giriniz: ");
	controlId = numberScanf(5);
	FILE *fp, *fpEx;
	char chr;
	for ( j = 0; j < 2; j++)
	{
		if (j) 
		{
			fp = fopen("ticari.txt", "r");
		}
		else
		{
			fp = fopen("bireysel.txt", "r");
		}
		if (fp != NULL)
		{
			chr = fgetc(fp);
			while (!feof(fp)) {
				if (chr == '*')
				{
					fscanf(fp, "%d", &recipient.musteriId);
					fscanf(fp, "%d", &recipient.musteriHesabi.hesapSayisi);
					fscanf(fp, "%s", recipient.musteriAdi);
					fscanf(fp, "%s", recipient.musteriAdi);
				}
				if (chr == '_')
				{
					for (i = 0; i < recipient.musteriHesabi.hesapSayisi; i++)
					{
						fscanf(fp, "%d", &recipient.musteriHesabi.hesapId);
						if (controlId == recipient.musteriHesabi.hesapId)
						{
							int k;
							for ( k = 0; k < strlen(recipient.musteriAdi); k++)
							{
								if (*(recipient.musteriAdi + k) == '.')
								{
									*(recipient.musteriAdi + k) = ' ';
								}
							}
							cont = 0;
							fclose(fp);
							break;
						}
						else
						{
							cont = 1;
						}
					}
				}
				if (cont == 0)
				{
					break;
				}
				chr = fgetc(fp);
				
			}
			fclose(fp);
		}
		if (cont==0)
		{
			break;
		}
	}
	if (cont==1)
	{
		printf("Girmis oldugunuz %d idli bir hesap yoktur...\n", controlId);
		backToAccountTransactionsChoice(sendOwner);
	}
	int amount, ekBakiye, surplus = 0, choice = 0;
	char fileName[25] = "hesaplar/", fileNameEx[20] = "hesaplar/", id[7];
	printf("Kullanacaniz hesap Id'nizi giriniz: ");
	sendOwner->musteriHesabi.hesapId = numberScanf(5);
	itoa(sendOwner->musteriId, id, 10);
	strcat(fileName, id);
	itoa(sendOwner->musteriHesabi.hesapId, id, 10);
	strcat(fileName, id);
	strcat(fileName, ".txt");
	fp = fopen(fileName, "r");
	itoa(sendOwner->musteriId, id, 10);
	strcat(fileNameEx, id);
	strcat(fileNameEx, ".txt");
	fpEx = fopen(fileNameEx, "r");
	if ((fp != 0) && (fpEx != 0))
	{
		fscanf(fp, "%d", &sendOwner->musteriHesabi.hesapBakiyesi);
		printf("Bakiyeniz:%d TL\n", sendOwner->musteriHesabi.hesapBakiyesi);
		printf("Yatiracaginiz tutari giriniz: ");
		amount = numberScanf(0);
		if(amount==0){
			printf("Havale edilecek para tutari 0 TL olamaz isleminiz iptal edilmistir...\n");
			fclose(fp);
			fclose(fpEx);
			backToAccountTransactionsChoice(sendOwner);
		}
		fscanf(fpEx, "%d", &ekBakiye);
		if ((sendOwner->musteriHesabi.hesapBakiyesi - amount) < 0)
		{
			surplus = (sendOwner->musteriHesabi.hesapBakiyesi - amount)*-1;
			sendOwner->musteriHesabi.hesapBakiyesi = 0;
			if ((ekBakiye - surplus) >= 0)
			{
				printf("Hesabin bakiyesi yetmiyor %d TL icin ek hesabi kullanalim mi\nEk hesap bakiyesi %d Tl (1.Evet/2.Hayir): ", surplus, ekBakiye);
				choice = selector(2);
				if (choice - 1)
				{
					printf("Isleminiz iptal edilmistir...\n");
					fclose(fp);
					fclose(fpEx);
					backToAccountTransactionsChoice(sendOwner);
				}
				else
				{
					ekBakiye -= surplus;
				}
			}
			else
			{
				printf("Hesabinizin ve ek hesabinizin bakiyesi yetersiz oldugundan isleminiz iptal edilmistir...\n");
				fclose(fp);
				fclose(fpEx);
				backToAccountTransactionsChoice(sendOwner);
			}
		}
		else
		{
			sendOwner->musteriHesabi.hesapBakiyesi -= amount;
		}
		fclose(fpEx);
		fpEx = fopen(fileNameEx, "w");
		fprintf(fpEx, "%d", ekBakiye);
		fclose(fpEx);
		FILE *fpTemp = fopen("temp.txt", "w");
		fprintf(fpTemp, "%d", sendOwner->musteriHesabi.hesapBakiyesi);
		while (!feof(fp)) {
			chr = fgetc(fp);
			if (feof(fp))
			{
				break;
			}
			fputc(chr, fpTemp);
		}
		int date = currentTime();
		day = date % 100;
		date /= 100;
		if (day < 10)
		{
			fprintf(fpTemp, "0");
		}
		fprintf(fpTemp, "%d.", day);
		month = date % 100;
		if (month < 10)
		{
			fprintf(fpTemp, "0");
		}
		fprintf(fpTemp, "%d.", month);
		year = date / 100;
		fprintf(fpTemp, "%d - Paraniz %s'in hesabina yatirildi %d TL", year, recipient.musteriAdi, amount);
		if (choice == 1)
		{
			fprintf(fpTemp, " - (%d TL ek hesaptan cekildi)", surplus);
		}
		fputc('\n', fpTemp);
		fclose(fpTemp);
		fclose(fp);
		fp = fopen(fileName, "w");
		fopen("temp.txt", "r");
		tempReplacer(fp, fpTemp);
	}
	else
	{
		printf("Girmis oldugunuz %d idli bir hesabiniz yoktur...\n", sendOwner->musteriHesabi.hesapId);
		sendOwner->musteriHesabi.hesapId = 0;
		backToAccountTransactionsChoice(sendOwner);
	}
	if (sendOwner->musteriTipi==1)
	{
		amount -= amount * 0.02;
	}
	strcpy(fileName,"hesaplar/");
	strcpy(fileNameEx, "hesaplar/");
	choice = 0;

	itoa(recipient.musteriId, id, 10);
	strcat(fileName, id);
	itoa(recipient.musteriHesabi.hesapId, id, 10);
	strcat(fileName, id);
	strcat(fileName, ".txt");
	fp = fopen(fileName, "r");
	itoa(recipient.musteriId, id, 10);
	strcat(fileNameEx, id);
	strcat(fileNameEx, ".txt");
	fpEx = fopen(fileNameEx, "r");
	if ((fp != 0) && (fpEx != 0))
	{
		fscanf(fpEx, "%d", &ekBakiye);
		fscanf(fp, "%d", &recipient.musteriHesabi.hesapBakiyesi);
		int debt = 500;
		recipient.musteriTipi=recipient.musteriId/ 100000;
		debt = (debt)*(recipient.musteriTipi);
		if ((ekBakiye - debt) < 0)
		{
			choice = 1;
			surplus = amount + (ekBakiye - debt);
			if (surplus < 0)
			{
				surplus = 0;
			}
			ekBakiye += amount - surplus;
		}
		else
		{
			surplus = amount;
		}
		recipient.musteriHesabi.hesapBakiyesi += surplus;
		fclose(fpEx);
		fpEx = fopen(fileNameEx, "w");
		fprintf(fpEx, "%d", ekBakiye);
		fclose(fpEx);
		char chr;
		FILE *fpTemp = fopen("temp.txt", "w");
		fprintf(fpTemp, "%d", recipient.musteriHesabi.hesapBakiyesi);
		while (!feof(fp)) {
			chr = fgetc(fp);
			if (feof(fp))
			{
				break;
			}
			fputc(chr, fpTemp);
		}
		if (day < 10)
		{
			fprintf(fpTemp, "0");
		}
		fprintf(fpTemp, "%d.", day);
		if (month < 10)
		{
			fprintf(fpTemp, "0");
		}
		fprintf(fpTemp, "%d.", month);
		fprintf(fpTemp, "%d - %s'in hesabindan para geldi %d TL", year,sendOwner->musteriAdi, amount);
		if (choice == 1)
		{
			fprintf(fpTemp, " - (%d TL ek hesaba yatirildi)", amount - surplus);
		}
		fputc('\n', fpTemp);
		fclose(fpTemp);
		fclose(fp);
		fp = fopen(fileName, "w");
		fopen("temp.txt", "r");
		tempReplacer(fp, fpTemp);
		printf("Isleminiz basari ile gerceklesti...\n");
		backToAccountTransactionsChoice(sendOwner);
	}
}

void report(musteri *sendOwner) {
	int bankIn, bankOut, bankTotal;
	FILE *fp;
	fp = fopen("rapor.txt", "r");
	fscanf(fp, "%d", &bankIn);
	fscanf(fp, "%d", &bankOut);
	fscanf(fp, "%d", &bankTotal);
	printf("Bankaya gelen para:\t%d TL\n",bankIn);
	printf("Bankadan giden para:\t%d TL\n", bankOut);
	printf("Bankadaki toplam para:\t%d TL\n", bankTotal);
	fclose(fp);
	backToAccountTransactionsChoice(sendOwner);
}

void  deleteAcc(musteri *sendOwner){
	int customerId,cont=0,contAcc=0,accountId,accountCount,exBalance,balance,ex=0;
	FILE *fp,*fpEx,*fpTemp=fopen("temp.txt","w");
	char chr,fileName[25]="hesaplar/", fileNameEx[25] = "hesaplar/",id[7];
	printf("Musterinin idsini giriniz: ");
	customerId = numberScanf(6);
	if ((customerId / 100000) == 1)
	{
		fp = fopen("bireysel.txt", "r");
	}
	else if ((customerId / 100000) == 2)
	{
		fp = fopen("ticari.txt", "r");
	}
	else
	{
		printf("Girdiginiz musteri id numarasi yok...\n");
		backToAccountTransactionsChoice(sendOwner);
		fp = fopen("ticari.txt", "r");
	}
	if (fp != NULL)
	{
		while (!feof(fp))
		{
			chr = fgetc(fp);
			if (chr == '*')
			{
				fscanf(fp, "%d", &cont);
				fscanf(fp,"%d",&accountCount);
				if (cont == customerId)
				{
					cont = 1;
					break;
				}
				else{
					accountCount=0;
					cont=0;
				}
			}
		}
		fclose(fp);
	}
	if (cont)
	{
		if (accountCount == 1)
		{
			itoa(customerId, id, 10);
			strcat(fileNameEx, id);
			strcat(fileNameEx, ".txt");
			fpEx = fopen(fileNameEx, "r");
			fscanf(fpEx, "%d", &exBalance);
			fclose(fpEx);
			if ((((customerId / 100000) == 1) && (exBalance != 500)) || (((customerId / 100000) == 2) && (exBalance != 1000)))
			{
				printf("1 tane hesabiniz oldugundan ek hesap borcu odenmeden hesabiniz silinmez...\n");
				backToAccountTransactionsChoice(sendOwner);
			}
		}
		printf("Musterinin hesap id giriniz: ");
		accountId = numberScanf(5);
		itoa(customerId, id, 10);
		strcat(fileName, id);
		itoa(accountId, id, 10);
		strcat(fileName, id);
		strcat(fileName, ".txt");
		fp = fopen(fileName, "r");
		if (fp!=0)
		{
			
			fscanf(fp, "%d", &balance);
			fclose(fp);
			if (balance != 0)
			{
				printf("Hesabi sifirlamadan hesabiniz silinmez...\n");
				backToAccountTransactionsChoice(sendOwner);
			}
			else
			{
				remove(fileName);
				if ((customerId / 100000) == 1)
				{
					fp = fopen("bireysel.txt", "r");
				}
				else
				{
					fp = fopen("ticari.txt", "r");
				}
				if (fp != NULL)
				{
					while (!feof(fp))
					{
						chr = fgetc(fp);
						if (feof(fp))
						{
							break;
						}
						fputc(chr,fpTemp);
						if (chr == '*')
						{
							fscanf(fp, "%d", &cont);
							fscanf(fp, "%d", &accountCount);
							if (cont == customerId)
							{
								accountCount--;
								ex=1;
							}
							fprintf(fpTemp, "\n%d", cont);
							fprintf(fpTemp, "\n%d", accountCount);
						}
						if ((chr == '_')&&(ex==1))
						{
							int i;
							for ( i = 0; i < accountCount+1; i++)
							{
								fscanf(fp, "%d", &contAcc);
								if (contAcc != accountId)
								{
									fprintf(fpTemp, "\n%d", contAcc);
								}
							}
							ex = 0;
						}
					}
					fclose(fp);
					fclose(fpTemp);
				}
				fpTemp = fopen("temp.txt","r");
				if ((customerId / 100000) == 1)
				{
					fp = fopen("bireysel.txt", "w");
				}
				else
				{
					fp = fopen("ticari.txt", "w");
				}
				tempReplacer(fp,fpTemp);
				printf("Isleminiz tamamlandi...\n");
				backToAccountTransactionsChoice(sendOwner);
			}
		}
	}
	else
	{
		printf("Girdiginiz musteri id numarasi yok...\n");
		backToAccountTransactionsChoice(sendOwner);
		fp = fopen("ticari.txt", "r");
	}
}

void record(musteri *sendOwner) {
	char chr;
	FILE *fp;
	printf("Dekontunu istediginiz hesabinizin idsini giriniz: ");
	sendOwner->musteriHesabi.hesapId = numberScanf(5);
	char dosyaYolu[25] = "hesaplar/",id[7];
	itoa(sendOwner->musteriId,id,10);
	strcat(dosyaYolu,id);
	itoa(sendOwner->musteriHesabi.hesapId, id, 10);
	strcat(dosyaYolu, id);
	strcat(dosyaYolu,".txt");
	fp = fopen(dosyaYolu,"r");
	if (fp!=0)
	{
		FILE *fpRecord = fopen("dekont.txt","w");
		int empty,day,month,year,dayF=22,monthF=05,yearF=2019, dayL=23, monthL=05, yearL=2019,calendar,calendarF,calendarL,now=currentTime();
		fscanf(fp,"%d",&empty);
		do
		{
			printf("Dekontun baslangic tarihini gün.ay.yil seklinde giriniz: ");
			scanf("%d.%d.%d", &dayF, &monthF, &yearF);
			calendarF = dayF + (monthF * 100) + (yearF * 10000);
		} while (calendarF>=now);
		
		do
		{
			printf("Dekontun bitis tarihini gün.ay.yil seklinde giriniz: ");
			scanf("%d.%d.%d", &dayL, &monthL, &yearL);
			calendarL = dayL + (monthL * 100) + (yearL * 10000);
		} while ((calendarF > now)||(calendarL< calendarF));
		
		for (;;)
		{
			if (feof(fp))
			{
				break;
			}
			fscanf(fp, "%d.", &day);
			fscanf(fp, "%d.", &month);
			fscanf(fp, "%d", &year);
			calendar =year* 10000 + month* 100 + day;
			if ((calendar>=calendarF)&&(calendar <= calendarL))
			{
				if (feof(fp))
				{
					break;
				}
				if (day<10)
				{
					fprintf(fpRecord,"0");
					printf("0");
				}
				fprintf(fpRecord,"%d.",day);
				printf("%d.", day);
				if (month < 10)
				{
					fprintf(fpRecord, "0");
					printf("0");
				}
				fprintf(fpRecord, "%d.%d", month,year);
				printf("%d.%d", month, year);
				while ((chr = fgetc(fp))!='\n')
				{
					if (feof(fp))
					{
						break;
					}
					fputc(chr,fpRecord);
					putchar(chr);
				}
				fputc(chr, fpRecord);
				putchar(chr);
			}
			else
			{
				while ((chr = fgetc(fp)) != '\n')
				{
					if (feof(fp))
					{
						break;
					}
				}
			}
			
		}
		fclose(fp);
		fclose(fpRecord);
		backToAccountTransactionsChoice(sendOwner);
	}
	else
	{
		printf("%d idli bir hesabiniz yoktur...",sendOwner->musteriHesabi.hesapId);
		backToAccountTransactionsChoice(sendOwner);
		return 0;
	}
	getch();
	return 0;
}
