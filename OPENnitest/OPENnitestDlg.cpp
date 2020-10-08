
// OPENnitestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPENnitest.h"
#include "OPENnitestDlg.h"
#include "afxdialogex.h"
#include <OpenNI.h>
#include "Viewer.h"
#include "OniSampleUtilities.h"
#include <opencv2\opencv.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "stdio.h"
#include <fstream>
#include <string>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SAMPLE_READ_WAIT_TIMEOUT 2000 //2000ms
using namespace openni;
using namespace std;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COPENnitestDlg dialog



COPENnitestDlg::COPENnitestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENNITEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COPENnitestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COPENnitestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &COPENnitestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &COPENnitestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &COPENnitestDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// COPENnitestDlg message handlers

BOOL COPENnitestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COPENnitestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COPENnitestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COPENnitestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COPENnitestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	AllocConsole();
	FILE* fpc;
	FILE *fpcl, *fs;
	freopen_s(&fpc, "CONOUT$", "w", stdout);
	freopen_s(&fpc, "CONIN$", "r", stdin);
	
	int x, y;

	try {

		openni::OpenNI::initialize();



		openni::Device device;

		auto ret = device.open(openni::ANY_DEVICE);

		if (ret != openni::STATUS_OK) {

			throw std::runtime_error("");

		}



		openni::VideoStream depthStream;

		depthStream.create(device, openni::SensorType::SENSOR_DEPTH);

		depthStream.start();



		std::vector<openni::VideoStream*> streams;

		streams.push_back(&depthStream);



		cv::Mat depthImage;



		while (1) {

			int changedIndex;

			openni::OpenNI::waitForAnyStream(&streams[0], streams.size(), &changedIndex);

			if (changedIndex == 0) {

				openni::VideoFrameRef depthFrame;

				depthStream.readFrame(&depthFrame);

				if (depthFrame.isValid()) {

					depthImage = cv::Mat(depthStream.getVideoMode().getResolutionY(),

						depthStream.getVideoMode().getResolutionX(),

						CV_16U, (char*)depthFrame.getData());



					// 0-10000mmまでのデータを0-255にする
					cv::Mat mes_img, res_img;
					depthImage.convertTo(depthImage, CV_8U, 255.0 / 10000);
					cv::cvtColor(depthImage, res_img, CV_GRAY2BGR);
					cv::rectangle(res_img, cv::Rect((depthImage.cols - 1) / 2 - 10, (depthImage.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position

					fpcl = fopen("data.xyz", "wt");
					for (y = 0; y < depthImage.rows; y++)
					{
						for (x = 0; x < depthImage.cols; x++)
						{
							UINT16 d = depthImage.at<UINT16>(y, x);
							if (d > 0)
							{
								int xc = -1 * (x - 320)*d / 333;
								int yc = (y - 240)*d / 333;
								fprintf(fpcl, "%d %d %d \n", xc, yc, (int)d);
							}
							
						}
					}
					fclose(fpcl);
					// 中心点の距離を表示する

					/*auto videoMode = depthStream.getVideoMode();



					int centerX = videoMode.getResolutionX() / 2;

					int centerY = videoMode.getResolutionY() / 2;

					int centerIndex = (centerY * videoMode.getResolutionX()) + centerX;



					short* depth = (short*)depthFrame.getData();



					std::stringstream ss;

					ss << "Center Point :" << depth[centerIndex];

					cv::putText(depthImage, ss.str(), cv::Point(0, 50),

						cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 0, 0), 1);
						*/


					cv::imshow("Depth Camera", depthImage);
					cv::imshow("res_img", res_img);
					cv::imwrite("depth.png", depthImage);

				}

			}



			int key = cv::waitKey(10);

			if (key == 'q') {

				break;

			}

		}

	}

	catch (std::exception&) {

		std::cout << openni::OpenNI::getExtendedError() << std::endl;

	}

	
}


void COPENnitestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	AllocConsole();
	FILE* fpc;
	freopen_s(&fpc, "CONOUT$", "w", stdout);
	freopen_s(&fpc, "CONIN$", "r", stdin);
	FILE *fp;

	CString filepath;
	CString filter("*.png|*.png");
	CFileDialog selDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	cv::Mat image;
	cv::Mat measure_img;
	cv::Mat result;
	cv::Mat separate_img;
	if (selDlg.DoModal() == IDOK)
	{
		filepath = selDlg.GetPathName();//can move to where want to select png file
		CStringA csTempmb(filepath.GetBuffer(0));//CStringA:char type is stored
		image = cv::imread(csTempmb.GetBuffer(0));//loaded image:csTempmb.GetBuffer(0)
		image.convertTo(measure_img, CV_8U, 255 / 4500);
		cv::cvtColor(image, result, CV_GRAY2BGR);//
		//cv::rectangle(result, cv::Rect((image.cols - 1) / 2 - 10, (image.rows - 1) / 2 - 10, 20, 20), cv::Scalar(0, 0, 255), 2);//get the center position
		//image.copyTo(separate_img);
		//UINT16 d = image.at<UINT16>((image.rows - 1) / 2, (image.cols - 1) / 2);
		//string windows_name = "depth image";
		cv::imshow("depth", image);
		cv::imshow("result", result);
		//cv::imshow("result image", result);
		//std::cout << "rows=x" << img.rows;
		//std::cout<< "cols=y" << img.cols;

		/*int x, y,a,b,c;
		int size = 0;
		FILE* fp;
		FILE *fs;
		FILE *fw;
		fp = fopen("datas.xyz", "wt");
		fs = fopen("data.xyz", "r+");
		fw = fopen("intensity.xyz", "wt");
		std::vector<int> f_in;
		std::vector<int>x_val;
		std::vector<int>y_val;
		std::vector<int>z_val;

				while ((fscanf(fs, "%d %d %d", &a, &b, &c )) != EOF)
				{
					x_val.push_back(a);
					y_val.push_back(b);
					z_val.push_back(c);
			
					fprintf(fp, "%d %d %d \n", x_val, y_val, z_val);

				}*/

				

	}
}


void COPENnitestDlg::OnBnClickedButton4()
{
	AllocConsole();
	FILE* fpc;
	FILE *fpcl, *fs;
	freopen_s(&fpc, "CONOUT$", "w", stdout);
	freopen_s(&fpc, "CONIN$", "r", stdin);

	int x, y;
	// TODO: Add your control notification handler code here
	try {

		openni::OpenNI::initialize();



		openni::Device device;

		auto ret = device.open(openni::ANY_DEVICE);

		if (ret != openni::STATUS_OK) {

			throw std::runtime_error("");

		}



		openni::VideoStream colorStream;

		colorStream.create(device, openni::SensorType::SENSOR_COLOR);

		colorStream.start();



		std::vector<openni::VideoStream*> streams;

		streams.push_back(&colorStream);



		cv::Mat colorImage;



		while (1) {

			int changedIndex;

			openni::OpenNI::waitForAnyStream(&streams[0], streams.size(), &changedIndex);

			if (changedIndex == 0) {

				openni::VideoFrameRef colorFrame;

				colorStream.readFrame(&colorFrame);

				if (colorFrame.isValid()) {

					colorImage = cv::Mat(colorStream.getVideoMode().getResolutionY(),

						colorStream.getVideoMode().getResolutionX(),

						CV_8UC3, (char*)colorFrame.getData());

				}



				cv::cvtColor(colorImage, colorImage, CV_BGR2RGB);

				cv::imshow("Color Camera", colorImage);



				int key = cv::waitKey(10);

				if (key == 'q') {

					break;

				}

			}

		}

	}

	catch (std::exception&) {

		std::cout << openni::OpenNI::getExtendedError() << std::endl;

	}
}

