#include <stdio.h>
#include <string.h>

#define MAX_BUF  256

#define IV  0b10110001
#define KEY 0b11001011
#define CTR 0b00110101

void encode(unsigned char*, unsigned char*, int);
void decode(unsigned char*, unsigned char*, int);

unsigned char processCtr(unsigned char, unsigned char);
unsigned char encryptByte(unsigned char, unsigned char, unsigned char);
unsigned char decryptByte(unsigned char, unsigned char, unsigned char);

unsigned char getBit(unsigned char, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);

void encode(unsigned char*pt, unsigned char*ct, int numBytes)
{
    unsigned char ctr = CTR, prev = IV, key = KEY;
    
    for(int i = 0; i < numBytes; i++)
    {
        ctr = processCtr(ctr, key);
        ct[i] = encryptByte(pt[i], ctr, prev);
        prev = ct[i];
        
        ctr++;
    }
}
void decode(unsigned char*ct, unsigned char*pt, int numBytes)
{
    unsigned char ctr = CTR, prev = IV, key = KEY;
    
    for(int i = 0; i < numBytes; i++)
    {
        ctr = processCtr(ctr, key);
        pt[i] = decryptByte(ct[i], ctr, prev);
        prev = ct[i];
        
        ctr++;
    }
}

unsigned char processCtr(unsigned char ctr, unsigned char key)
{
    unsigned char temp = ctr;
    int i = ((ctr % 2) == 0) ? 0 : 1;
    
    for(; i < 8; i += 2)
    {
        unsigned char currBit = getBit(ctr, i) ^ getBit(key, i);
        
        if(currBit == 0)
            temp = clearBit(temp, i);
        else
            temp = setBit(temp, i);
    }
    
    return temp;
}
unsigned char encryptByte(unsigned char pt, unsigned char ctr, unsigned char prev)
{
    unsigned char temp = 0;
    
    for(int i = 0; i < 8; i ++)
    {
        if(getBit(ctr, i) != 0)
        {
            unsigned char bit1 = (getBit(pt, i) != 0) ? 1 : 0;
            unsigned char bit2 = (getBit(prev, i) != 0) ? 1 : 0;
            unsigned char currBit = bit1 ^ bit2;
            
            if(currBit != 0)
                temp = setBit(temp, i);
        }
        else
        {
            unsigned char bit1 = (getBit(pt, i) != 0) ? 1 : 0;
            unsigned char bit2 = (getBit(prev, 7 - i) != 0) ? 1 : 0;
            unsigned char currBit = bit1 ^ bit2;
            
            if(currBit != 0)
                temp = setBit(temp, i);
        }
    }
    
    return temp;
}
unsigned char decryptByte(unsigned char ct, unsigned char ctr, unsigned char prev)
{
    unsigned char temp = 0;
    
    for(int i = 0; i < 8; i ++)
    {
        if(getBit(ctr, i) != 0)
        {
            unsigned char bit1 = (getBit(ct, i) != 0) ? 1 : 0;
            unsigned char bit2 = (getBit(prev, i) != 0) ? 1 : 0;
            unsigned char currBit = bit1 ^ bit2;
            
            if(currBit != 0)
                temp = setBit(temp, i);
        }
        else
        {
            unsigned char bit1 = (getBit(ct, i) != 0) ? 1 : 0;
            unsigned char bit2 = (getBit(prev, 7 - i) != 0) ? 1 : 0;
            unsigned char currBit = bit1 ^ bit2;
            
            if(currBit != 0)
                temp = setBit(temp, i);
        }
    }
    
    return temp;
}

int main()
{
  unsigned char str[2048];
  unsigned char chiper[2048];
  unsigned char output[2048];
  int  choice;
  int numBytes;
  printf("\nYou may:\n");
  printf("  (1) Encrypt a message \n");
  printf("  (2) Decrypt a message \n");
  printf("  (0) Exit\n");
  printf("\n  what is your selection: ");
    
  fgets(str, sizeof(str), stdin);
  sscanf(str, "%d", &choice);

  if (choice == 0)
    return 0;

  switch (choice) {

    case 1:
        printf("\nEnter Plain Test : ");
        
        fgets(str, sizeof(str), stdin);
        
        numBytes = strlen(str) - 1;
        
        encode(str, output, numBytes);
        
        printf("Ciphertext : \n");
        for(int i = 0; i < numBytes; i++)
        {
            printf("%u ", (unsigned char)output[i]);
        }
        
     break;

      case 2:
        printf("\nEnter Cipher text : ");
        
        numBytes = 0;
        int tempValue = 0;
        
        do {
            scanf("%d", &tempValue);
            
            if(tempValue == -1)
                break;
            
            str[numBytes++] = (unsigned char)tempValue;
            
        } while(str[numBytes - 1] != -1);
        
        decode(str, output, numBytes);
        
        printf("\nPlain Test : \n");
        for(int i = 0; i < numBytes; i++)
        {
            printf("%c", (unsigned char)output[i]);
        }
        
      break;
  }
    
  return(0);
}

/*
  Function:  getBit
  Purpose:   retrieve value of bit at specified position
       in:   character from which a bit will be returned
       in:   position of bit to be returned
   return:   value of bit n in character c (0 or 1)
*/
unsigned char getBit(unsigned char c, int n)
{
    return (c & (1 << n));
}

/*
  Function:  setBit
  Purpose:   set specified bit to 1
       in:   character in which a bit will be set to 1
       in:   position of bit to be set to 1
   return:   new value of character c with bit n set to 1
*/
unsigned char setBit(unsigned char c, int n)
{
    return (c | (1 << n));
}

/*  Function:  clearBit
  Purpose:   set specified bit to 0
       in:   character in which a bit will be set to 0
       in:   position of bit to be set to 0
   return:   new value of character c with bit n set to 0
*/
unsigned char clearBit(unsigned char c, int n)
{
    return (c & ~(1 << n));
}


