#include <stdio.h>
 #include <stdlib.h>
#include <stdint.h>
#include<string.h>

 #pragma pack(1)


typedef uint8_t  BYTE;
typedef uint32_t DWORD;
typedef int32_t  LONG;
typedef uint16_t WORD;

typedef struct
{
    BYTE ftype1;    //B for bmp file 
    BYTE ftype2;     //M for bmp file
    DWORD fsize;     
    WORD garbo1;
    WORD garbo2; 
    DWORD offset;   //location of first pixel
}BMPheader;

typedef struct
{
    DWORD hsize;
    LONG w;             //number of pixels in a row
    LONG h;             //number of pixels in a column
    WORD Cplanes;
    WORD bitcount;
    DWORD comp;         //compression (0 for bmp files)
    DWORD hsizeImage;
    LONG hdensity;
    LONG vdensity;
    DWORD clrused;
    DWORD clrimp;
}INFOheader;


typedef struct
{
    BYTE b;     //blue component of pixel
    BYTE g;     //green component of pixel
    BYTE r;     //red component of pixel
}RGBarr;

int main(void)
{
    char infile[]="";
    char outfile[]="encrypted_";

    printf("Enter File name: ");
    scanf("%s", infile);

    int opt;
    printf("Press 1 for encryption\nPress 2 for decryption\n");
    scanf("%d",&opt);

    int columns, rows;

    if(opt==1){

        FILE *inptr = fopen(infile, "rb");
        
        if (inptr == NULL)
        {
            printf("Could not open image.\n");
            return 2;
        }

        strcat(outfile, infile);

        FILE *outptr = fopen(outfile, "wb");
        if (outptr == NULL)
        {
            fclose(inptr);
            fprintf(stderr, "Could not create %s.\n", outfile);
            return 3;
        }

        char msg[2000];
        printf("enter word to encrypt:\n");
        fflush(stdin);
        scanf("%[^\n]s",msg);

        BMPheader bmp;
        fread(&bmp, sizeof(BMPheader), 1, inptr);

        INFOheader bmpinfo;
        fread(&bmpinfo, sizeof(INFOheader), 1, inptr);

        fwrite(&bmp, sizeof(BMPheader), 1, outptr);

        fwrite(&bmpinfo, sizeof(INFOheader), 1, outptr);

        columns=bmpinfo.h;
        rows=bmpinfo.w;
        unsigned int total_pxl=rows*columns; 

        BYTE pxl[total_pxl][3];

        for(int i = 0; i <= total_pxl; i++)
        {
            RGBarr rgb;
            fread(&rgb, sizeof(RGBarr), 1, inptr);	
                pxl[i][0]=rgb.b;
                pxl[i][1]=rgb.g;
                pxl[i][2]=rgb.r;
        }

        pxl[1500][1]=strlen(msg);
        //printf("%d value of pxl:", pxl[1500][1]);
    

        int cou=0;
        for(int i = 0; i <strlen(msg)/3; i++)
        {
            for(int j=0; j<3; j++)
            {
                pxl[i][j]=msg[cou];
                cou++;
            }           
        }


            
        for(int i = 0; i < 3; i++)
        {
            fwrite(&pxl, sizeof(pxl), 1, outptr);
        }

        fclose(inptr);
        fclose(outptr);

    }


    else

    {

        FILE *inptr = fopen(infile, "rb");
        
        if (inptr == NULL)
        {
            printf("Could not open image.\n");
            return 2;
        }

        BMPheader bmp;
        fread(&bmp, sizeof(BMPheader), 1, inptr);

        INFOheader bmpinfo;
        fread(&bmpinfo, sizeof(INFOheader), 1, inptr);
        
        unsigned int total_pxl=bmpinfo.w*bmpinfo.h;

        BYTE pxl[total_pxl][3];

        for(int i = 0; i <= total_pxl; i++)
        {
            RGBarr rgb;
            fread(&rgb, sizeof(RGBarr), 1, inptr);	
                pxl[i][0]=rgb.b;
                pxl[i][1]=rgb.g;
                pxl[i][2]=rgb.r;
        }

        int cou=0;
        for(int i = 0; i <pxl[1500][1]/3; i++)
        {
            for(int j=0; j<3; j++)
            {
                printf("%c", pxl[i][j]);
                cou++;
            }           
        }

        fclose(inptr);
        
    }

return 0;

}