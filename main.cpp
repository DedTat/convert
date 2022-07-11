
/// Convert bmp from 8 bit to 24 bit

#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

BOOL Convert256To24 (char *fin, char *fout)

 {
     string sFileName;
     BITMAPFILEHEADER bmpFileHeader;
     BITMAPINFOHEADER bmpInfoHeader;
     int Width, Height;
     RGBQUAD Palette[256];
     RGBTRIPLE *inBuf;
     BYTE *outBuf;
     HANDLE hInputFile, hOutFile;
     DWORD RW;
     cout << "Enter the full name, please: ";
     cin >> sFileName;

     DWORD OffBits;
     int i, j;

     hInputFile = CreateFile (sFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
     if (hInputFile == INVALID_HANDLE_VALUE)
         return 0;

     hOutFile = CreateFile ("Result.bmp", GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
     if (hOutFile == INVALID_HANDLE_VALUE)
     {
         CloseHandle (hInputFile);
         return 0;
     }

     // Прочтем данные
     ReadFile (hInputFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
     ReadFile (hInputFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);
     //ReadFile (hIn, Palette, 256 * sizeof(RGBQUAD), &RW, NULL);

     // Установим указатель на начало растра
     SetFilePointer (hInputFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);
     Width = bmpInfoHeader.biWidth;
     Height = bmpInfoHeader.biHeight;
    // OffBits = bfh.bfOffBits;

     // Выделим память
     inBuf = new RGBTRIPLE [Width];
     outBuf = new BYTE [Width];

     // Заполним заголовки
     bmpFileHeader.bfOffBits = sizeof (bmpFileHeader) + sizeof (bmpInfoHeader);                               // Не будем писать палитру
     bmpInfoHeader.biBitCount = 24;
     bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + 4 * Width * Height + Height * (Width % 4);    // Размер файла

     // А остальное не меняется
     // Запишем заголовки
     WriteFile (hOutFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
     WriteFile (hOutFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

     // Начнем преобразовывать
     for (i = 0; i < Height; i++)
     {
         ReadFile (hInputFile, inBuf, sizeof(RGBTRIPLE)*Width, &RW, NULL);
         for (j = 0; j < Width; j++)
         {
             outBuf[i].rgbRed = Palette[inBuf[j]].rgbRed;
             //outBuf[j].rgbtGreen = Palette[inBuf[j]].rgbGreen;
             //outBuf[j].rgbtBlue = Palette[inBuf[j]].rgbBlue;
            /* Palette[i].rgbRed = Palette[inBuf[j]].rgbRed;
             Palette[i].rgbGreen = Palette[inBuf[j]].rgbGreen;
             Palette[i].rgbBlue = Palette[inBuf[j]].rgbBlue;
            */
         }
         WriteFile (hOutFile, outBuf, sizeof(RGBTRIPLE) * Width, &RW, NULL);

         // Пишем мусор для выравнивания
         WriteFile (hOutFile, Palette, Width % 4, &RW, NULL);
         SetFilePointer (hInputFile, (3 * Width) % 4, NULL, FILE_CURRENT);
     }

     delete inBuf;
     delete outBuf;
     CloseHandle (hInputFile);
     CloseHandle (hOutFile);
    // return;
    cout << "Updating has come to the end successfully!";
system("pause");
 }
    cout << "Hello world!" << endl;
    return 0;
}
