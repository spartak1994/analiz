//---------------------------------------------------------------------------

#include <vcl.h>
#include <cmath>
#include <string.h>
#include <System.SysUtils.hpp>
#include <dir.h>
#include <stdio.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	int iFileHandle;
	int iNewFileHandle;
	__int64 iFileLength=0;
	__int64 iBytesRead=0;
	__int64 iBytesWrite=0;
	float mas[80000];
	float mas_amp[20]={0};
	int count=0;
	int start_luch=StrToInt(Edit3->Text);
	int end_luch=StrToInt(Edit4->Text);
	int frag_size=StrToInt(Edit5->Text);
	int min_frag_size=StrToInt(Edit6->Text);
	iFileHandle = FileOpen( Edit1->Text, fmOpenRead);
	if (iFileHandle == -1)
	{
		Button1->Caption="Ошибка открытия";
		Button1->Enabled=0;
	}
	if(iFileHandle>0)
	{
		//Edit5->Text=(UnicodeString)"C:\\KMB-2-12\\Pasha\\test_stat_2_b_fast.txt";
		if(!FileExists(Edit2->Text))
			{
				iNewFileHandle = FileCreate(Edit2->Text);
				char str[]="№_ob\t№_lch\t№_frg\tmax\t\t\tavg\t\t\tavg_kv\tcount\n";
				iBytesWrite+=FileWrite(iNewFileHandle, str,sizeof(char)*42);
				FileClose(iNewFileHandle);
			}

		iNewFileHandle = FileOpen(Edit2->Text, fmOpenWrite);
		if(iNewFileHandle<=0) Button1->Caption="Ошибка записи";
		else Button1->Caption="Анализирует";
		Button1->Enabled=0;
		iFileLength=FileSeek(iFileHandle,(__int64) 0, 2);
		FileSeek(iFileHandle,(__int64) 0, 0);
		FileSeek(iNewFileHandle,(__int64) 0, 2);
		int i=0;//для управления циклом
		int j=0;//для подсчета номера внутри фрагмента
		int div=0;//для опрделения числа фрагментов внутри луча, кроме не целого фрагмента
		int ost=0;//для определения размера нецелого фрагмента
		int oborot=1;//номер оборота
		int luch=start_luch;// номер луча
		int frag=1;//номер фрагмента
		while(iBytesRead<iFileLength)
		{
			//i++;
			iBytesRead+=FileRead(iFileHandle, mas,80000*sizeof(float));
			count=0;//в этой переменной храним количество не нулевых амплитуд в текущем луче
			for(int k=0; k<80000; k++)if(mas[k]>0)count++;
			//Label3->Caption=(UnicodeString)count;
			div=count/frag_size;
			ost=count%frag_size;
			//Label3->Caption=(UnicodeString)div;

			int k=0;
			for(int t=0; t<div; t++)
			{
				j=0;
				while(j<frag_size)
				{
					if(mas[k]>0)
					{
						mas_amp[j]=mas[k];
						j++;
					}
					k++;
				}
				//к этому месту мы поймали 20 не нулевых из одного луча
				float sum=0;
				float avg=0;
				float sum_kv=0;
				float avg_kv=0;
				float p=0;
				float max_amp=0;
				float min_amp=mas_amp[0];
				for(int r=0; r<frag_size;r++)
				{
					if(mas_amp[r]>max_amp)max_amp=mas_amp[r];
					if(mas_amp[r]<min_amp)min_amp=mas_amp[r];
					sum+=mas_amp[r];
					sum_kv+=pow(mas_amp[r],2);
				}
				avg =(float)sum/frag_size;
				p =(float)sum_kv/frag_size;
				avg_kv=sqrt(p-pow(avg,2));
				//bel3->Caption=(UnicodeString)avg_kv;
				//тут уже вычислена статистика для отловленых 20 ненулевых амплитуд

				char* str_ob=new char[10];
				sprintf(str_ob, "%d",oborot);  //переводим в строку номер оборота
				int len_ob=StrLen(str_ob);
				char* str_luch=new char[10];
				sprintf(str_luch, "%d",luch);  //переводим в строку номер луча
				int len_luch = StrLen(str_luch);
				char tab='\t';
				char* str_frag = new char[10];
				sprintf(str_frag, "%d",frag); //переводим в строку номер фрагмента
				int len_frag = StrLen(str_frag);
				char endl='\n';
				char* str_max=new char[10];
				sprintf(str_max, "%f",max_amp); //переводим в строку максимальную амплитуду из фрагмента
				int len_max = StrLen(str_max);
				char* str_avg=new char[10];
				sprintf(str_avg, "%f",avg); //переводим в строку среднюю амплитуду по фрагменту
				int len_avg = StrLen(str_avg);
				char* str_avg_kv=new char[10];
				sprintf(str_avg_kv, "%f",avg_kv);//переводим в строку среднюю квадратичную амплитуду по фрагменту
				int len_avg_kv = StrLen(str_avg_kv);
				char* str_c=new char[10];
				sprintf(str_c, "%d",count);//переводим в строку среднюю квадратичную амплитуду по фрагменту
				int len_c = StrLen(str_c);

				iBytesWrite+=FileWrite(iNewFileHandle, str_ob ,len_ob);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_luch ,len_luch);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_frag ,len_frag);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_max ,len_max);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_avg ,len_avg);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_avg_kv ,len_avg_kv);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_c ,len_c);
				iBytesWrite+=FileWrite(iNewFileHandle, &endl ,sizeof(char));


				frag++;
			}
			if(ost>min_frag_size)
			{
				j=0;
				while(j<ost)
				{
					if(mas[k]>0)
					{
						mas_amp[j]=mas[k];
						j++;
					}
					k++;
				}
				//отловили остаточный фрагмент

				float sum=0;
				float avg=0;
				float sum_kv=0;
				float avg_kv=0;
				float p=0;
				float max_amp=0;
				float min_amp=mas_amp[0];
				for(int r=0; r<ost;r++)
				{
					if(mas_amp[r]>max_amp)max_amp=mas_amp[r];
					if(mas_amp[r]<min_amp)min_amp=mas_amp[r];
					sum+=mas_amp[r];
					sum_kv+=pow(mas_amp[r],2);
				}
				avg =(float)sum/ost;
				p =(float)sum_kv/ost;
				avg_kv=sqrt(p-pow(avg,2));
				//Label3->Caption=(UnicodeString)avg_kv;
				//тут уже вычислена статистика для отловленых 20 ненулевых амплитуд

				char* str_ob=new char[10];
				sprintf(str_ob, "%d",oborot);  //переводим в строку номер оборота
				int len_ob=StrLen(str_ob);
				char* str_luch=new char[10];
				sprintf(str_luch, "%d",luch);  //переводим в строку номер луча
				int len_luch = StrLen(str_luch);
				char tab='\t';
				char* str_frag = new char[10];
				sprintf(str_frag, "%d",frag); //переводим в строку номер фрагмента
				int len_frag = StrLen(str_frag);
				char endl='\n';
				char* str_max=new char[10];
				sprintf(str_max, "%f",max_amp); //переводим в строку максимальную амплитуду из фрагмента
				int len_max = StrLen(str_max);
				char* str_avg=new char[10];
				sprintf(str_avg, "%f",avg); //переводим в строку среднюю амплитуду по фрагменту
				int len_avg = StrLen(str_avg);
				char* str_avg_kv=new char[10];
				sprintf(str_avg_kv, "%f",avg_kv);//переводим в строку среднюю квадратичную амплитуду по фрагменту
				int len_avg_kv = StrLen(str_avg_kv);
				char* str_c=new char[10];
				sprintf(str_c, "%d",ost);//переводим в строку среднюю квадратичную амплитуду по фрагменту
				int len_c = StrLen(str_c);

				iBytesWrite+=FileWrite(iNewFileHandle, str_ob ,len_ob);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_luch ,len_luch);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_frag ,len_frag);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_max ,len_max);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_avg ,len_avg);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_avg_kv ,len_avg_kv);
				iBytesWrite+=FileWrite(iNewFileHandle, &tab ,sizeof(char));
				iBytesWrite+=FileWrite(iNewFileHandle, str_c ,len_c);
				iBytesWrite+=FileWrite(iNewFileHandle, &endl ,sizeof(char));


				frag++;
			}
			luch++;
			frag=1;
			if(luch>end_luch)
			{
				luch=start_luch;
				oborot++;
				char str[]="№_ob\t№_lch\t№_frg\tmax\t\t\tavg\t\t\tavg_kv\t\tcount\n";
				iBytesWrite+=FileWrite(iNewFileHandle, str,sizeof(char)*43);
			}
		}

		//Label3->Caption=(UnicodeString)count;
		Button1->Caption="Готово";
		FileClose(iFileHandle);
		FileClose(iNewFileHandle);
	}
}
//---------------------------------------------------------------------------
