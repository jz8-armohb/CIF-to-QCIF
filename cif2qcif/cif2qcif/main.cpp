#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	FILE* cifPtr;	// CIF文件指针
	FILE* qcifPtr;	// QCIF文件指针
	const char* cifName = argv[1];	// CIF文件名
	const char* qcifName = argv[2];	// QCIF文件名
	int cifW = 352;
	int cifH = 288;
	int qcifW = cifW / 2;
	int qcifH = cifH / 2;
	int cifSize;	// CIF文件的总字节数
	int qcifSize;	// QCIF文件的总字节数
	int cifFrameSize = cifW * cifH * 3 / 2;	//CIF文件每帧的字节数
	int qcifFrameSize = cifFrameSize / 4;
	int frameNum;
	unsigned char* cifYBuf = NULL;	// 存储CIF文件一帧中Y分量的缓冲区
	unsigned char* cifUBuf = NULL;	// 存储CIF文件一帧中U分量的缓冲区
	unsigned char* cifVBuf = NULL;	// 存储CIF文件一帧中V分量的缓冲区
	unsigned char* qcifYBuf = NULL;	// 存储QCIF文件一帧中Y分量的缓冲区
	unsigned char* qcifUBuf = NULL;	// 存储QCIF文件一帧中U分量的缓冲区
	unsigned char* qcifVBuf = NULL;	// 存储QCIF文件一帧中V分量的缓冲区

	// 打开文件
	if (fopen_s(&cifPtr, cifName, "rb") == 0)
	{
		cout << "Successfully opened " << cifName << "." << endl;
	}
	else
	{
		cout << "WARNING! Failed to open " << cifName << "." << endl;
		exit(0);
	}
	if (fopen_s(&qcifPtr, qcifName, "wb") == 0)
	{
		cout << "Successfully opened " << qcifName << "." << endl;
	}
	else
	{
		cout << "WARNING! Failed to open " << qcifName << "." << endl;
		exit(0);
	}

	// 计算原RGB图像总字节数和总帧数
	fseek(cifPtr, 0L, SEEK_END);
	cifSize = ftell(cifPtr);
	rewind(cifPtr);
	cout << "The space that " << cifName << " accounts for is " << cifSize << " Bytes = " << (double)cifSize / 1024 / 1024 << " MB." << endl;
	qcifSize = cifSize / 2;
	frameNum = cifSize / cifFrameSize;
	cout << "There are " << frameNum << " frames in this video clip." << endl;

	// 按帧建立缓冲区
	cifYBuf = new unsigned char[cifFrameSize * 2 / 3];
	cifUBuf = new unsigned char[cifFrameSize / 6];
	cifVBuf = new unsigned char[cifFrameSize / 6];
	qcifYBuf = new unsigned char[qcifFrameSize * 2 / 3];
	qcifUBuf = new unsigned char[qcifFrameSize / 6];
	qcifVBuf = new unsigned char[qcifFrameSize / 6];


	// 逐帧处理
	for (int i = 0; i < frameNum; i++)	// 控制帧数
	{
		fread(cifYBuf, sizeof(unsigned char), cifFrameSize * 2 / 3, cifPtr);	// 读入第i帧的Y分量
		fread(cifUBuf, sizeof(unsigned char), cifFrameSize / 6, cifPtr);	// 读入第i帧的U分量
		fread(cifVBuf, sizeof(unsigned char), cifFrameSize / 6, cifPtr);	// 读入第i帧的V分量

		// Y分量下变换
		for (int j = 0; j < cifH; j += 2)
		{
			for (int k = 0; k < cifW; k += 2)
			{
				qcifYBuf[j / 2 * cifW / 2 + k / 2] = cifYBuf[j * cifW + k];
			}
		}
		// U、V分量下变换
		for (int j = 0; j < cifH / 2; j += 2)
		{
			for (int k = 0; k < cifW / 2; k += 2)
			{
				qcifUBuf[j / 2 * cifW / 4 + k / 2] = cifUBuf[j * cifW / 2 + k];
				qcifVBuf[j / 2 * cifW / 4 + k / 2] = cifVBuf[j * cifW / 2 + k];
			}
		}

		fwrite(qcifYBuf, sizeof(unsigned char), qcifFrameSize * 2 / 3, qcifPtr);	// 写入第i帧的Y分量
		fwrite(qcifUBuf, sizeof(unsigned char), qcifFrameSize / 6, qcifPtr);	// 写入第i帧的Y分量
		fwrite(qcifVBuf, sizeof(unsigned char), qcifFrameSize / 6, qcifPtr);	// 写入第i帧的Y分量
	}

	delete[]cifYBuf;
	delete[]cifUBuf;
	delete[]cifVBuf;
	delete[]qcifYBuf;
	delete[]qcifUBuf;
	delete[]qcifVBuf;
	fclose(cifPtr);
	fclose(qcifPtr);
}