#include<iostream>  
#include<stdio.h>   
#include<stdlib.h>   
#include<string.h>
using namespace std;  

static unsigned char char_to_hex( unsigned char x )   
{   
	return (unsigned char)(x > 9 ? x + 55: x + 48);   
}   

static int is_alpha_number_char( unsigned char c )   
{   
	if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') )   
		return 1;   
	return 0;   
}   

//url编码实现   
void urlencode( unsigned char * src, int  src_len, unsigned char * dest, int  dest_len )   
{   
	unsigned char ch;   
	int  len = 0;   

	while (len < (dest_len - 4) && *src)   
	{   
		ch = (unsigned char)*src;   
		if (*src == ' ')   
		{   
			*dest++ = '+';   
		}   
		else if (is_alpha_number_char(ch) || strchr("=!~*'()", ch))   
		{   
			*dest++ = *src;   
		}   
		else   
		{   
			*dest++ = '%';   
			*dest++ = char_to_hex( (unsigned char)(ch >> 4) );   
			*dest++ = char_to_hex( (unsigned char)(ch % 16) );   
		}    
		++src;   
		++len;   
	}   
	*dest = 0;   
	return ;   
}   



//解url编码实现   

unsigned char* urldecode(const char* encd,unsigned char* decd)   
{   
	int j,i;   
	char *cd =(char*) encd;   
	char p[2];   
	unsigned int num;   
	j=0;   

	for( i = 0; i < strlen(cd); i++ )   
	{   
		memset( p, '\0', 2 );   
		if( cd[i] != '%' )   
		{   
			decd[j++] = cd[i];   
			continue;   
		}   

		p[0] = cd[++i];   
		p[1] = cd[++i];   

		p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0);   
		p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0);   
		decd[j++] = (unsigned char)(p[0] * 16 + p[1]);   

	}   
	decd[j] = '\0';   

	return decd;   
}  

char* ReadFile(char *filename)  
{  
	char *buffer = NULL;  
	int string_size,read_size;  
	FILE *handler = fopen(filename,"r");  

	if (handler)  
	{  
		fseek(handler,0,SEEK_END);  
		//offset from the first to the last byte, or in other words, filesize  
		string_size = ftell (handler);  
		//go back to the start of the file  
		rewind(handler);  

		//allocate a string that can hold it all  
		buffer = (char*) malloc (sizeof(char) * (string_size + 1) );  
		//read it all in one operation  
		read_size = fread(buffer,sizeof(char),string_size,handler);  
		//fread doesnt set it so put a \0 in the last position  
		//and buffer is now officialy a string  
		buffer[string_size+1] = '\0';  

		if (string_size != read_size) {  
			//something went wrong, throw away the memory and set  
			//the buffer to NULL  
			free(buffer);  
			buffer = NULL;  
		}  
	}  

	return buffer;  
}  

void getIPFromDecode( char * url, char * IP )   
{
	if(url == NULL)
		return;
	char* IPBeg = NULL;
	const char* IPEnd = NULL;

	const char* flag = "\">";
	if((IPBeg = strstr(url, flag)) != NULL)
	{
		IPEnd = strstr(IPBeg, "</");
		if(IPEnd == NULL)
			return;
	}

	strncpy(IP, IPBeg + strlen(flag), IPEnd - IPBeg -strlen(flag));
}

char* getPortFromHtml(char* html, char* port)
{
	if(html == NULL)
		return NULL;
	char* strStrBeg = NULL;
	char* strStrEnd = NULL;

	const char* flag = "<td align=\"center\">";
	if((strStrBeg = strstr(html, flag)) == NULL)
		return NULL;

	strStrEnd = strstr(strStrBeg + strlen(flag), "<");

	if(strStrEnd == NULL)
		return NULL;

	strncpy(port, strStrBeg + strlen(flag), strStrEnd - strStrBeg -strlen(flag));

	return strStrEnd;
}

char* getUrlFromHtml(char* html, char* url)
{
	if(html == NULL)
		return NULL;
	char* strStrBeg = NULL;
	char* strStrEnd = NULL;

	const char* flag = "IPDecode(\"";
	if((strStrBeg = strstr(html, flag)) == NULL)
		return NULL;

	strStrEnd = strstr(strStrBeg, ")");

	if(strStrEnd == NULL)
		return NULL;

	strncpy(url, strStrBeg + strlen(flag), strStrEnd - strStrBeg -strlen(flag) - 1);

	return strStrEnd;
}

int main(int argc, const char** argv)
{
	if(argc < 2)
		return 0;

	char file[100] = {0};
	strcpy(file, argv[1]);
	if(strlen(file) < 2)
		return 0;

	char* html = ReadFile(file);
	char url[1024] = {0};
	while((html = getUrlFromHtml(html, url)) != NULL)
	{
		unsigned char decode[1024];
		memset( decode, '\0', 1024 );   

		urldecode(url, decode);

		char IP[16] = {0};
		getIPFromDecode((char*)decode, IP);

		char port[5] = {0};
		getPortFromHtml(html, port);

		printf("%s:%s\n", IP, port);
	}

	if(html)
		delete html;

	return 1;
}

