#include "stdafx.h"
#include "StudentInfoReflesh.h"

#define MODULE_NAME	_T("CStudentInfoReflesh")

CStudentInfoReflesh::CStudentInfoReflesh()
{
	InitializeCriticalSection(&this->m_carSignalLock);
	m_refleshEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	m_bStartExam = false;
	m_bEndExam = false;
	m_bPass = false;
	m_nDisplayDelays = 0;
	m_nCurrentScore = EXAM_TOTAL_SCORE;
	m_mapJudgeInfos.clear();

	m_nStartXmStatus = 0;
	m_nEndXmStatus = 0;
	m_nKskm = KSKM_2;
	m_nKM2ItemCount = ItemCount::ITEM_COUNT_5;
	m_bBigCar = false;

	SetEvent(m_refleshEvent);
}

CStudentInfoReflesh::~CStudentInfoReflesh()
{
	if (NULL != this->m_studentInfoRefleshThread && this->m_studentInfoRefleshThread->IsMainThreadRunning())
	{
		this->m_studentInfoRefleshThread->StopMainThread();
		delete this->m_studentInfoRefleshThread;
	}

	CloseHandle(this->m_refleshEvent);

	//清理锁以及其他
	DeleteCriticalSection(&this->m_carSignalLock);
}

void CStudentInfoReflesh::StartWork()
{
	LoadConfig();

	m_studentInfoRefleshThread = CreateIThreadInstance(StudentInfoRefleshThreadProc, LPVOID(this));
	m_studentInfoRefleshThread->StartMainThread();
}

void CStudentInfoReflesh::SetCarSignal(CarSignal signal)
{
	if (m_bStartExam)
	{
		EnterCriticalSection(&this->m_carSignalLock);
		m_carSignal = signal;
		LeaveCriticalSection(&this->m_carSignalLock);
	}
}

void CStudentInfoReflesh::Handle17C51(StudentInfo studentInfo)
{
	m_studentInfo = studentInfo;

	m_mapJudgeInfos.clear();
	m_bStartExam = true;
	m_bEndExam = false;
	m_nDisplayDelays = 0;
	m_nCurrentScore = EXAM_TOTAL_SCORE;

	m_nStartXmStatus = 0;
	m_nEndXmStatus = 0;

	SetEvent(m_refleshEvent);
}

void CStudentInfoReflesh::Handle17C52(int xmNo, wstring xmName)
{
	//fix me
	//编号修改为宏定义
	if (KSKMType::KSKM_2 == m_nKskm)	//科目二
	{
		int nType = xmNo / 1000;

		if (!m_bBigCar)
		{
			if (201 == nType)
			{
				m_nStartXmStatus |= ITEM_DCRK_FLAG;
			}
			else if (204 == nType)
			{
				m_nStartXmStatus |= ITEM_CFTC_FLAG;
			}
			else if (203 == nType)
			{
				m_nStartXmStatus |= ITEM_BPQB_FLAG;
			}
			else if (206 == nType)
			{
				m_nStartXmStatus |= ITEM_QXXS_FLAG;
			}
			else if (207 == nType)
			{
				m_nStartXmStatus |= ITEM_ZJZW_FLAG;
			}
			else if (214 == nType)
			{
				m_nStartXmStatus |= ITEM_MNSD_FLAG;
			}
			else if (215 == nType)
			{
				m_nStartXmStatus |= ITEM_YWSH_FLAG;
			}
			else if (216 == nType)
			{
				m_nStartXmStatus |= ITEM_YWSH_FLAG;
			}
		}
		else
		{
			if (202 == nType)
			{
				m_nStartXmStatus |= 0x0001;
			}
			else if (204 == nType)
			{
				m_nStartXmStatus |= 0x0002;
			}
			else if (203 == nType)
			{
				m_nStartXmStatus |= 0x0004;
			}
			else if (205 == nType)
			{
				m_nStartXmStatus |= 0x0008;
			}
			else if (209 == nType)
			{
				m_nStartXmStatus |= 0x0010;
			}
			else if (208 == nType)
			{
				m_nStartXmStatus |= 0x0020;
			}
			else if (207 == nType)
			{
				m_nStartXmStatus |= 0x0040;
			}
			else if (206 == nType)
			{
				m_nStartXmStatus |= 0x0080;
			}
			else if (210 == nType)
			{
				m_nStartXmStatus |= 0x0100;
			}
			else if (211 == nType)
			{
				m_nStartXmStatus |= 0x0200;
			}
			else if (213 == nType)
			{
				m_nStartXmStatus |= 0x0400;
			}
			else if (216 == nType)
			{
				m_nStartXmStatus |= 0x0800;
			}
			else if (215 == nType)
			{
				m_nStartXmStatus |= 0x1000;
			}
			else if (214 == nType)
			{
				m_nStartXmStatus |= 0x2000;
			}
			else if (212 == nType)
			{
				m_nStartXmStatus |= 0x4000;
			}
			else if (217 == nType)
			{
				m_nStartXmStatus |= 0x8000;
			}
		}
	}
	else  //科目三
	{
		if (201 == xmNo)
		{
			m_nStartXmStatus |= ITEM_SCZB_FLAG;
		}
		else if (202 == xmNo)
		{
			m_nStartXmStatus |= ITEM_QB_FLAG;
		}
		else if (203 == xmNo)
		{
			m_nStartXmStatus |= ITEM_ZXXS_FLAG;
		}
		else if (204 == xmNo)
		{
			m_nStartXmStatus |= ITEM_BGCD_FLAG;
		}
		else if (205 == xmNo)
		{
			//没有处理
		}
		else if (206 == xmNo)
		{
			m_nStartXmStatus |= ITEM_RXHD_FLAG;
		}
		else if (207 == xmNo)
		{
			m_nStartXmStatus |= ITEM_XXQY_FLAG;
		}
		else if (208 == xmNo)
		{
			m_nStartXmStatus |= ITEM_GJCZ_FLAG;
		}
		else if (209 == xmNo)
		{
			m_nStartXmStatus |= ITEM_HC_FLAG;
		}
		else if (210 == xmNo)
		{
			m_nStartXmStatus |= ITEM_CC_FLAG;
		}
		else if (211 == xmNo)
		{
			m_nStartXmStatus |= ITEM_KBTC_FLAG;
		}
		else if (212 == xmNo)
		{
			m_nStartXmStatus |= ITEM_DT_FLAG;
		}
		else if (213 == xmNo)
		{
			m_nStartXmStatus |= ITEM_YJ_FLAG;
		}
		else if (214 == xmNo)
		{
			m_nStartXmStatus |= ITEM_ZZ_FLAG;
		}
		else if (215 == xmNo)
		{
			m_nStartXmStatus |= ITEM_YZ_FLAG;
		}
		else if (216 == xmNo)
		{
			m_nStartXmStatus |= ITEM_ZHIXING_FLAG;
		}
		else if (217 == xmNo)
		{
			m_nStartXmStatus |= ITEM_JJD_FLAG;
		}
	}

}

void CStudentInfoReflesh::Handle17C55(int xmNo, wstring xmName)
{
	//fix me
	//编号修改为宏定义
	if (KSKM_2 == m_nKskm)	//科目二
	{
		int nType = xmNo / 1000;

		if (!m_bBigCar)
		{
			if (201 == nType)
			{
				m_nEndXmStatus |= ITEM_DCRK_FLAG;
			}
			else if (204 == nType)
			{
				m_nEndXmStatus |= ITEM_CFTC_FLAG;
			}
			else if (203 == nType)
			{
				m_nEndXmStatus |= ITEM_BPQB_FLAG;
			}
			else if (206 == nType)
			{
				m_nEndXmStatus |= ITEM_QXXS_FLAG;
			}
			else if (207 == nType)
			{
				m_nEndXmStatus |= ITEM_ZJZW_FLAG;
			}
			else if (214 == nType)
			{
				m_nEndXmStatus |= ITEM_MNSD_FLAG;
			}
			else if (215 == nType)
			{
				m_nEndXmStatus |= ITEM_YWSH_FLAG;
			}
			else if (216 == nType)
			{
				m_nEndXmStatus |= ITEM_YWSH_FLAG;
			}
		}
		else
		{
			if (202 == nType)
			{
				m_nEndXmStatus |= 0x0001;
			}
			else if (204 == nType)
			{
				m_nEndXmStatus |= 0x0002;
			}
			else if (203 == nType)
			{
				m_nEndXmStatus |= 0x0004;
			}
			else if (205 == nType)
			{
				m_nEndXmStatus |= 0x0008;
			}
			else if (209 == nType)
			{
				m_nEndXmStatus |= 0x0010;
			}
			else if (208 == nType)
			{
				m_nEndXmStatus |= 0x0020;
			}
			else if (207 == nType)
			{
				m_nEndXmStatus |= 0x0040;
			}
			else if (206 == nType)
			{
				m_nEndXmStatus |= 0x0080;
			}
			else if (210 == nType)
			{
				m_nEndXmStatus |= 0x0100;
			}
			else if (211 == nType)
			{
				m_nEndXmStatus |= 0x0200;
			}
			else if (213 == nType)
			{
				m_nEndXmStatus |= 0x0400;
			}
			else if (216 == nType)
			{
				m_nEndXmStatus |= 0x0800;
			}
			else if (215 == nType)
			{
				m_nEndXmStatus |= 0x1000;
			}
			else if (214 == nType)
			{
				m_nEndXmStatus |= 0x2000;
			}
			else if (212 == nType)
			{
				m_nEndXmStatus |= 0x4000;
			}
			else if (217 == nType)
			{
				m_nEndXmStatus |= 0x8000;
			}
		}
	}
	else  //科目三
	{
		if (201 == xmNo)
		{
			m_nEndXmStatus |= ITEM_SCZB_FLAG;
		}
		else if (202 == xmNo)
		{
			m_nEndXmStatus |= ITEM_QB_FLAG;
		}
		else if (203 == xmNo)
		{
			m_nEndXmStatus |= ITEM_ZXXS_FLAG;
		}
		else if (204 == xmNo)
		{
			m_nEndXmStatus |= ITEM_BGCD_FLAG;
		}
		else if (205 == xmNo)
		{
			//没有处理
		}
		else if (206 == xmNo)
		{
			m_nEndXmStatus |= ITEM_RXHD_FLAG;
		}
		else if (207 == xmNo)
		{
			m_nEndXmStatus |= ITEM_XXQY_FLAG;
		}
		else if (208 == xmNo)
		{
			m_nEndXmStatus |= ITEM_GJCZ_FLAG;
		}
		else if (209 == xmNo)
		{
			m_nEndXmStatus |= ITEM_HC_FLAG;
		}
		else if (210 == xmNo)
		{
			m_nEndXmStatus |= ITEM_CC_FLAG;
		}
		else if (211 == xmNo)
		{
			m_nEndXmStatus |= ITEM_KBTC_FLAG;
		}
		else if (212 == xmNo)
		{
			m_nEndXmStatus |= ITEM_DT_FLAG;
		}
		else if (213 == xmNo)
		{
			m_nEndXmStatus |= ITEM_YJ_FLAG;
		}
		else if (214 == xmNo)
		{
			m_nEndXmStatus |= ITEM_ZZ_FLAG;
		}
		else if (215 == xmNo)
		{
			m_nEndXmStatus |= ITEM_YZ_FLAG;
		}
		else if (216 == xmNo)
		{
			m_nEndXmStatus |= ITEM_ZHIXING_FLAG;
		}
		else if (217 == xmNo)
		{
			m_nEndXmStatus |= ITEM_JJD_FLAG;
		}
	}
}

void CStudentInfoReflesh::Handle17C53(ERROR_DATA judgeInfo)
{
	int nIndex = m_mapJudgeInfos.size();
	if (nIndex < 5)	//最多展示5条扣分信息
	{
		m_mapJudgeInfos[nIndex] = judgeInfo;
	}

	m_nCurrentScore -= judgeInfo.ikcfs;
	if (m_nCurrentScore < 0)
	{
		m_nCurrentScore = 0;
	}
}

void CStudentInfoReflesh::Handle17C56(bool bPass, int nScore)
{
	m_bPass = bPass;
	m_nCurrentScore = nScore;
	m_bStartExam = false;
	m_bEndExam = true;
	m_nDisplayDelays = DISPLAY_DELAY_SECONDS;
}

BOOL CStudentInfoReflesh::StudentInfoRefleshThreadProc(LPVOID parameter, HANDLE stopEvent)
{
	L_INFO(_T("StudentInfoRefleshThreadProc Start\n"));

	CStudentInfoReflesh *studentInfoRefleshClass = (CStudentInfoReflesh*)parameter;

	Graphics graphics(studentInfoRefleshClass->m_DC.GetSafeHdc());
	graphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQuality);
	graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality);
	graphics.SetCompositingMode(Gdiplus::CompositingMode::CompositingModeSourceOver);

	CFont fontz;
	fontz.CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"宋体");
	studentInfoRefleshClass->m_DC.SelectObject(&fontz);
	studentInfoRefleshClass->m_DC.SetBkMode(TRANSPARENT);//透明
	studentInfoRefleshClass->m_DC.SetTextColor(RGB(255, 255, 255));

	while (true)
	{
		DWORD dwRet = WaitForSingleObject(studentInfoRefleshClass->m_refleshEvent, INFINITE);
		if (WAIT_OBJECT_0 == dwRet)
		{
			if (studentInfoRefleshClass->m_bStartExam || !studentInfoRefleshClass->m_bEndExam || studentInfoRefleshClass->m_nDisplayDelays > 0)
			{				
				//绘制背景
				studentInfoRefleshClass->DrawBackground(&graphics);
				studentInfoRefleshClass->DrawItemBackground(&graphics);

				if (studentInfoRefleshClass->m_bStartExam || studentInfoRefleshClass->m_nDisplayDelays > 0)	//考试中才展示这些信息
				{
					//绘制考生基本信息
					studentInfoRefleshClass->DrawStudentInfo(&graphics);

					//绘制扣分信息
					studentInfoRefleshClass->DrawJudgement(&graphics);

					//绘制实时信号
					studentInfoRefleshClass->DrawSignal(&graphics);

					//绘制成绩
					studentInfoRefleshClass->DrawScore(&graphics);

					//绘制实时项目状态
					studentInfoRefleshClass->DrawCurrentItem(&graphics);

					//考试结束时显示考试是否通过
					if (studentInfoRefleshClass->m_bEndExam && studentInfoRefleshClass->m_nDisplayDelays > 0)
					{
						studentInfoRefleshClass->DrawResult(&graphics);
					}
				}

				//刷新四合一界面
				studentInfoRefleshClass->Reflesh();

				Sleep(500);

				//考试结束后，继续显示一段时间
				if (studentInfoRefleshClass->m_nDisplayDelays > 0)
				{
					studentInfoRefleshClass->m_nDisplayDelays -= 1;
				}

				SetEvent(studentInfoRefleshClass->m_refleshEvent);
			}
		}

	}

EXIT:
	L_INFO(_T("StudentInfoRefleshThreadProc Exit\n"));
	return TRUE;
}

void CStudentInfoReflesh::LoadConfig()
{
	wstring wsEnvConfPath = m_wsProgramPath + CONF_PATH_ENV;

	//考试科目
	m_nKskm = GetPrivateProfileInt(CONF_SECTION_CONFIG, CONF_KEY_KSKM, 2, wsEnvConfPath.c_str());

	//是否是大车程序
	int nBigCar = GetPrivateProfileInt(CONF_SECTION_CONFIG, CONF_KEY_BIGCAR, 0, wsEnvConfPath.c_str());
	if (1 == nBigCar)
	{
		m_bBigCar = true;
	}

	//小车科目二项目数量
	if (KSKMType::KSKM_2 == m_nKskm)
	{
		m_nKM2ItemCount = GetPrivateProfileInt(CONF_SECTION_CONFIG, CONF_KEY_KM2_ITEM_COUNT, 
			ItemCount::ITEM_COUNT_5, wsEnvConfPath.c_str());
	}
}

//绘制背景
void CStudentInfoReflesh::DrawBackground(Graphics *graphics)
{
	try
	{
		wstring wsStuBackground = m_wsProgramPath + IMG_PATH_STUDENT_BACKGROUND;
		if (!CWinUtils::FileExists(wsStuBackground))
		{
			L_ERROR(_T("wsStuBackground not exist, file name = %s\n"), wsStuBackground.c_str());
			return;
		}

		Image *imgStuBackground = Image::FromFile(wsStuBackground.c_str());
		int width = imgStuBackground->GetWidth();
		int height = imgStuBackground->GetHeight();
		graphics->DrawImage(imgStuBackground, Rect(0, 0, m_nWidth, m_nHeight), 0, 0, width, height, UnitPixel);

		delete imgStuBackground;
	}
	catch (...)
	{
		L_ERROR(_T("DrawBackground catch an error.\n"));
	}
}

//绘制项目牌背景
void CStudentInfoReflesh::DrawItemBackground(Graphics *graphics)
{
	if (KSKMType::KSKM_2 == m_nKskm)
	{
		DrawKM2Background(graphics);
	}
	else if (KSKMType::KSKM_3 == m_nKskm)
	{
		DrawKM3Background(graphics);
	}
}

void CStudentInfoReflesh::DrawKM3Background(Graphics *graphics)
{
	try
	{
		int imgWidth = 60;
		int splitWidth = DISPLAY_CHAN_WIDTH / 3 * 2 / ItemCount::ITEM_COUNT_16;
		int x = (splitWidth - imgWidth) / 2;
		int y = 295;

		//上车准备
		DrawNormalItem(graphics, x + 0 * splitWidth, y, imgWidth, imgWidth, ITEM_SCZB);
		
		//起步
		DrawNormalItem(graphics, x + 1 * splitWidth, y, imgWidth, imgWidth, ITEM_QB);

		//直线行驶
		DrawNormalItem(graphics, x + 2 * splitWidth, y, imgWidth, imgWidth, ITEM_ZXXS);

		//加减档
		DrawNormalItem(graphics, x + 3 * splitWidth, y, imgWidth, imgWidth, ITEM_JJD);

		//变更车道
		DrawNormalItem(graphics, x + 4 * splitWidth, y, imgWidth, imgWidth, ITEM_BGCD);
		
		//靠边停车
		DrawNormalItem(graphics, x + 5 * splitWidth, y, imgWidth, imgWidth, ITEM_KBTC);

		//直行通过路口
		DrawNormalItem(graphics, x + 6 * splitWidth, y, imgWidth, imgWidth, ITEM_ZX);

		//左转
		DrawNormalItem(graphics, x + 7 * splitWidth, y, imgWidth, imgWidth, ITEM_ZZ);

		//右转
		DrawNormalItem(graphics, x + 8 * splitWidth, y, imgWidth, imgWidth, ITEM_YZ);

		//人行横道
		DrawNormalItem(graphics, x + 9 * splitWidth, y, imgWidth, imgWidth, ITEM_RXHD);

		//学校区域
		DrawNormalItem(graphics, x + 10 * splitWidth, y, imgWidth, imgWidth, ITEM_XXQY);

		//公交车站
		DrawNormalItem(graphics, x + 11 * splitWidth, y, imgWidth, imgWidth, ITEM_GJCZ);

		//会车
		DrawNormalItem(graphics, x + 12 * splitWidth, y, imgWidth, imgWidth, ITEM_HC);

		//超车
		DrawNormalItem(graphics, x + 13 * splitWidth, y, imgWidth, imgWidth, ITEM_CC);

		//掉头
		DrawNormalItem(graphics, x + 14 * splitWidth, y, imgWidth, imgWidth, ITEM_DT);

		//夜间
		DrawNormalItem(graphics, x + 15 * splitWidth, y, imgWidth, imgWidth, ITEM_YJ);
	}
	catch (...)
	{
		L_ERROR(_T("DrawItemBackground catch an error.\n"));
	}
}

void CStudentInfoReflesh::DrawKM2Background(Graphics *graphics)
{
	try
	{
		int imgWidth = 60;
		int splitWidth = DISPLAY_CHAN_WIDTH / 3 * 2 / m_nKM2ItemCount;
		int x = (splitWidth - imgWidth) / 2;
		int y = 295;

		//大部分地方科目二都是5个项目
		if (ItemCount::ITEM_COUNT_5 == m_nKM2ItemCount || ItemCount::ITEM_COUNT_7 == m_nKM2ItemCount)
		{
			//倒车入库
			DrawNormalItem(graphics, x + 0 * splitWidth, y, imgWidth, imgWidth, ITEM_DCRK);
			//侧方停车
			DrawNormalItem(graphics, x + 1 * splitWidth, y, imgWidth, imgWidth, ITEM_CFTC);
			//半坡起步
			DrawNormalItem(graphics, x + 2 * splitWidth, y, imgWidth, imgWidth, ITEM_BPQB);
			//曲线行驶
			DrawNormalItem(graphics, x + 3 * splitWidth, y, imgWidth, imgWidth, ITEM_QXXS);
			//直角转弯
			DrawNormalItem(graphics, x + 4 * splitWidth, y, imgWidth, imgWidth, ITEM_ZJZW);

			if (ItemCount::ITEM_COUNT_7 == m_nKM2ItemCount)
			{
				//雨雾湿滑
				DrawNormalItem(graphics, x + 5 * splitWidth, y, imgWidth, imgWidth, ITEM_YWSH);
				//模拟隧道
				DrawNormalItem(graphics, x + 6 * splitWidth, y, imgWidth, imgWidth, ITEM_MNSD);
			}
		}
	}
	catch (...)
	{
		L_ERROR(_T("DrawKM2Background catch an error.\n"));
	}
}

void CStudentInfoReflesh::DrawNormalItem(Graphics *graphics, int x, int y, int width, int height, wstring wsItem)
{
	wstring wsPath = m_wsProgramPath + FILE_PATH_ITEM + wsItem + _T("_Normal.png");
	if (!CWinUtils::FileExists(wsPath))
	{
		L_ERROR(_T("file not exist : %s\n"), wsPath.c_str());
		return;
	}

	Image *imgItem = Image::FromFile(wsPath.c_str());
	graphics->DrawImage(imgItem, Rect(x, y, width, height));
	delete imgItem;
}

void CStudentInfoReflesh::DrawEnterItem(Graphics *graphics, int x, int y, int width, int height, wstring wsItem)
{
	wstring wsPath = m_wsProgramPath + FILE_PATH_ITEM + wsItem + _T("_Enter.png");
	if (!CWinUtils::FileExists(wsPath))
	{
		L_ERROR(_T("file not exist : %s\n"), wsPath.c_str());
		return;
	}

	Image *imgItem = Image::FromFile(wsPath.c_str());
	graphics->DrawImage(imgItem, Rect(x, y, width, height));
	delete imgItem;
}

void CStudentInfoReflesh::DrawLeaveItem(Graphics *graphics, int x, int y, int width, int height, wstring wsItem)
{
	wstring wsPath = m_wsProgramPath + FILE_PATH_ITEM + wsItem + _T("_Leave.png");
	if (!CWinUtils::FileExists(wsPath))
	{
		L_ERROR(_T("file not exist : %s\n"), wsPath.c_str());
		return;
	}

	Image *imgItem = Image::FromFile(wsPath.c_str());
	graphics->DrawImage(imgItem, Rect(x, y, width, height));
	delete imgItem;
}

void CStudentInfoReflesh::DrawStudentInfo(Graphics *graphics)
{
	try
	{
		//绘制考生照片
		DrawPhoto(graphics);

		Gdiplus::Font myFont(FONT_MICROSOFT_YAHEI, 18, FontStyleRegular, UnitPoint);
		StringFormat format;
		format.SetAlignment(StringAlignmentNear);
		SolidBrush blackBrush(Color(0, 0, 0));

		int x = 155;
		int y = 2;
		int splitHeight = 28;
		int width = 500;

		//绘制考生信息
		wstring wsInfo = m_studentInfo.wsCarNo + _T("-") + m_studentInfo.wsCarType + _T("-") + m_studentInfo.wsRemarks;
		wstring wsName = _T("姓名：") + m_studentInfo.wsName;
		wstring wsGender = _T("性别：") + m_studentInfo.wsGender;
		wstring wsDate = _T("考试日期：") + m_studentInfo.wsDate;
		wstring wsSerialNo = _T("业务流水：") + m_studentInfo.wsSerialNo;
		wstring wsID = _T("身份证号：") + m_studentInfo.wsID;
		wstring wsDrivingSchool = _T("培训机构：") + m_studentInfo.wsDrivingSchool;
		wstring wsExamReason = _T("考试原因：") + m_studentInfo.wsExamReason;
		wstring wsExaminer = _T("考试员：") + m_studentInfo.wsExaminer;
		wstring wsDayCount = _T("考试次数：第 ") + m_studentInfo.wsDayCount + _T(" 次");
		graphics->DrawString(wsInfo.c_str(), wsInfo.length(), &myFont,
			RectF(x, y + 0 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsName.c_str(), wsName.length(), &myFont,
			RectF(x, y + 1 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsGender.c_str(), wsGender.length(), &myFont,
			RectF(x, y + 2 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsDate.c_str(), wsDate.length(), &myFont,
			RectF(x, y + 3 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsSerialNo.c_str(), wsSerialNo.length(), &myFont,
			RectF(x, y + 4 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsID.c_str(), wsID.length(), &myFont,
			RectF(x, y + 5 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsDrivingSchool.c_str(), wsDrivingSchool.length(), &myFont,
			RectF(x, y + 6 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsExamReason.c_str(), wsExamReason.length(), &myFont,
			RectF(x, y + 7 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsExaminer.c_str(), wsExaminer.length(), &myFont,
			RectF(x, y + 8 * splitHeight, width, splitHeight), &format, &blackBrush);
		graphics->DrawString(wsDayCount.c_str(), wsDayCount.length(), &myFont,
			RectF(x, y + 9 * splitHeight, width, splitHeight), &format, &blackBrush);
		
	}
	catch (...)
	{
		L_ERROR(_T("DrawStudentInfo catch an error.\n"));
	}
}

void CStudentInfoReflesh::DrawJudgement(Graphics *graphics)
{
	try
	{
		Gdiplus::Font myFont(FONT_MICROSOFT_YAHEI, 15, FontStyleRegular, UnitPoint);
		StringFormat format;
		format.SetAlignment(StringAlignmentNear);
		SolidBrush blackBrush(Color(0, 0, 0));

		int x = 605;
		int y = 150;
		int splitHeight = 26;
		int width = 600;

		for (int i = 0; i < m_mapJudgeInfos.size(); i++)
		{
			ERROR_DATA judgeInfo = m_mapJudgeInfos[i];
			
			int yDisplay = y + i * splitHeight;
			wstring wsNo = CStringUtils::Format(_T("%d"), i + 1);
			wstring wsDeduction = CStringUtils::Format(_T("%d"), judgeInfo.ikcfs);
			string sDes = CStringUtils::Format("%s", judgeInfo.errorlx);
			wstring wsDes = _T("");
			CStringUtils::ASCII2Unicode(sDes, wsDes);

			if (!wsNo.empty() && !wsDes.empty() && !wsDeduction.empty())
			{
				graphics->DrawString(wsNo.c_str(), wsNo.length(), &myFont,
					RectF(x, yDisplay, width, splitHeight), &format, &blackBrush);
				graphics->DrawString(wsDes.c_str(), wsDes.length(), &myFont,
					RectF(x + 55, yDisplay, width, splitHeight), &format, &blackBrush);
				graphics->DrawString(wsDeduction.c_str(), wsDeduction.length(), &myFont,
					RectF(x + 590, yDisplay, width, splitHeight), &format, &blackBrush);
			}
		}
	}
	catch (...)
	{
		L_ERROR(_T("DrawJudgement catch an error.\n"));
	}
}

void CStudentInfoReflesh::DrawPhoto(Graphics *graphics)
{
	try
	{
		wstring wsPhotoName = m_wsProgramPath + FILE_PATH_PHOTO + CStringUtils::Format(PHOTO_ID_FORMAT, m_nCarNo);
		if (CWinUtils::FileExists(wsPhotoName))
		{
			Image *imgPhoto = Image::FromFile(wsPhotoName.c_str());
			int width = imgPhoto->GetWidth();
			int height = imgPhoto->GetHeight();
			graphics->DrawImage(imgPhoto, Rect(5, 5, 140, 140), 0, 0, width, height, UnitPixel);

			delete imgPhoto;
		}
		else
		{
			L_TRACE(_T("file not exist : %s\n"), wsPhotoName.c_str());
		}

		wstring wsLoginPhotoName = m_wsProgramPath + FILE_PATH_PHOTO + 
			CStringUtils::Format(PHOTO_LOGIN_FORMAT, m_nCarNo);
		if (CWinUtils::FileExists(wsLoginPhotoName))
		{
			Image *imgLoginPhoto = Image::FromFile(wsLoginPhotoName.c_str());
			int width = imgLoginPhoto->GetWidth();
			int height = imgLoginPhoto->GetHeight();
			graphics->DrawImage(imgLoginPhoto, Rect(5, 148, 140, 140), 0, 0, width, height, UnitPixel);

			delete imgLoginPhoto;
		}
		else
		{
			L_TRACE(_T("file not exist : %s\n"), wsPhotoName.c_str());
		}
	}
	catch (...)
	{
		L_ERROR(_T("DrawPhoto catch an error.\n"));
	}
}

void CStudentInfoReflesh::DrawSignal(Graphics *graphics)
{
	try
	{
		EnterCriticalSection(&m_carSignalLock);
		CarSignal carSignal = m_carSignal;
		LeaveCriticalSection(&m_carSignalLock);

		//档位信号
		wstring wsGearPath = m_wsProgramPath + CStringUtils::Format(IMG_PATH_GEAR_FORMAT, m_carSignal.dw);
		if (!CWinUtils::FileExists(wsGearPath))
		{	
			L_ERROR(_T("file not exist : %s\n"), wsGearPath.c_str());
		}
		Image *imgGear = Image::FromFile(wsGearPath.c_str());
		graphics->DrawImage(imgGear, Rect(1130, 0, 156, 132));
		delete imgGear;

		wstring wsSignalPath = m_wsProgramPath + IMG_PATH_SIGNAL;
		if (!CWinUtils::FileExists(wsSignalPath))
		{
			L_ERROR(_T("file not exist : %s\n"), wsSignalPath.c_str());
			return;
		}
		Image *imgSignal = Image::FromFile(wsSignalPath.c_str());

		int x = 700;
		int y = 13;
		int splitSource = 40;
		int splitDest = 55;

		int xDest = 0;
		int yDest = y;
		int xSource = 0;
		int ySource = 0;

		//安全带
		xDest = x + 0 * splitDest;
		xSource = 0 * splitSource;
		ySource = carSignal.aqd * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//脚刹
		xDest = x + 1 * splitDest;
		xSource = 12 * splitSource;
		ySource = carSignal.js * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//手刹
		xDest = x + 2 * splitDest;
		xSource = 9 * splitSource;
		ySource = carSignal.ss * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//副刹
		xDest = x + 3 * splitDest;
		xSource = 13 * splitSource;
		ySource = carSignal.fs * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//喇叭
		xDest = x + 4 * splitDest;
		xSource = 7 * splitSource;
		ySource = carSignal.lb * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//左转向
		xDest = x + 5 * splitDest;
		xSource = 1 * splitSource;
		ySource = carSignal.zzx * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//右转向
		xDest = x + 6 * splitDest;
		xSource = 2 * splitSource;
		ySource = carSignal.yzx * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//远光灯
		xDest = x + 7 * splitDest;
		xSource = 4 * splitSource;
		ySource = carSignal.ygd* splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//换一行
		yDest += splitDest;

		//近光灯
		xDest = x + 0 * splitDest;
		xSource = 3 * splitSource;
		ySource = carSignal.jgd* splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//熄火
		xDest = x + 1 * splitDest;
		xSource = 10 * splitSource;
		ySource = carSignal.xh* splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//开关门
		xDest = x + 2 * splitDest;
		xSource = 6 * splitSource;
		ySource = carSignal.kgm* splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//离合
		xDest = x + 3 * splitDest;
		xSource = 16 * splitSource;
		ySource = carSignal.kgm* splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//警示灯
		xDest = x + 4 * splitDest;
		xSource = 25 * splitSource;
		ySource = carSignal.jsd * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//雨刮
		xDest = x + 5 * splitDest;
		xSource = 26 * splitSource;
		ySource = carSignal.yg * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//雾灯
		xDest = x + 6 * splitDest;
		xSource = 27 * splitSource;
		ySource = carSignal.wd * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		//示廓灯
		xDest = x + 7 * splitDest;
		xSource = 14 * splitSource;
		ySource = carSignal.skd * splitSource;
		graphics->DrawImage(imgSignal, Rect(xDest, yDest, splitSource, splitSource),
			xSource, ySource, splitSource, splitSource, UnitPixel);

		delete imgSignal;
	}
	catch (...)
	{
		L_ERROR(_T("DrawSignal catch an error.\n"));
	}
}

void CStudentInfoReflesh::DrawScore(Graphics *graphics)
{
	try
	{
		Gdiplus::Font myFont(FONT_MICROSOFT_YAHEI, 50, FontStyleRegular, UnitPoint);
		StringFormat format;
		format.SetAlignment(StringAlignmentFar);	//靠右对齐
		SolidBrush blackBrush(Color(0, 0, 200));

		wstring wsScore = CStringUtils::Format(_T("%d"), m_nCurrentScore);
		graphics->DrawString(wsScore.c_str(), wsScore.length(), &myFont,
			RectF(440, 5, 150, 150), &format, &blackBrush);

		Gdiplus::Font fontSmall(FONT_MICROSOFT_YAHEI, 25, FontStyleRegular, UnitPoint);
		format.SetAlignment(StringAlignmentNear);	//靠左对齐
		wstring wsDes = _T("分");
		graphics->DrawString(wsDes.c_str(), wsDes.length(), &fontSmall,
			RectF(580, 40, 200, 200), &format, &blackBrush);
	}
	catch (...)
	{
		L_ERROR(_T("DrawScore catch an error.\n"));
	}
}

void CStudentInfoReflesh::DrawCurrentItem(Graphics *graphics)
{
	if (KSKMType::KSKM_2 == m_nKskm)
	{
		DrawKM2EnterItem(graphics);
		DrawKM2LeaveItem(graphics);
	}
	else if (KSKMType::KSKM_3 == m_nKskm)
	{
		DrawKM3EnterItem(graphics);
		DrawKM3LeaveItem(graphics);
	}
}

void CStudentInfoReflesh::DrawKM3EnterItem(Graphics *graphics)
{
	int imgWidth = 60;
	int splitWidth = DISPLAY_CHAN_WIDTH / 3 * 2 / ItemCount::ITEM_COUNT_16;
	int x = (splitWidth - imgWidth) / 2;
	int y = 295;
	
	//上车准备
	if (m_nStartXmStatus & ITEM_SCZB_FLAG)
	{
		DrawEnterItem(graphics, x + 0 * splitWidth, y, imgWidth, imgWidth, ITEM_SCZB);
	}

	//起步
	if (m_nStartXmStatus & ITEM_QB_FLAG)
	{
		DrawEnterItem(graphics, x + 1 * splitWidth, y, imgWidth, imgWidth, ITEM_QB);
	}

	//直线行驶
	if (m_nStartXmStatus & ITEM_ZXXS_FLAG)
	{
		DrawEnterItem(graphics, x + 2 * splitWidth, y, imgWidth, imgWidth, ITEM_ZXXS);
	}

	//加减档
	if (m_nStartXmStatus & ITEM_JJD_FLAG)
	{
		DrawEnterItem(graphics, x + 3 * splitWidth, y, imgWidth, imgWidth, ITEM_JJD);
	}

	//变更车道
	if (m_nStartXmStatus & ITEM_BGCD_FLAG)
	{
		DrawEnterItem(graphics, x + 4 * splitWidth, y, imgWidth, imgWidth, ITEM_BGCD);
	}

	//靠边停车
	if (m_nStartXmStatus & ITEM_KBTC_FLAG)
	{
		DrawEnterItem(graphics, x + 5 * splitWidth, y, imgWidth, imgWidth, ITEM_KBTC);
	}

	//直行通过路口
	if (m_nStartXmStatus & ITEM_ZHIXING_FLAG)
	{
		DrawEnterItem(graphics, x + 6 * splitWidth, y, imgWidth, imgWidth, ITEM_ZX);
	}

	//左转
	if (m_nStartXmStatus & ITEM_ZZ_FLAG)
	{
		DrawEnterItem(graphics, x + 7 * splitWidth, y, imgWidth, imgWidth, ITEM_ZZ);
	}

	//右转
	if (m_nStartXmStatus & ITEM_YZ_FLAG)
	{
		DrawEnterItem(graphics, x + 8 * splitWidth, y, imgWidth, imgWidth, ITEM_YZ);
	}

	//人行横道
	if (m_nStartXmStatus & ITEM_RXHD_FLAG)
	{
		DrawEnterItem(graphics, x + 9 * splitWidth, y, imgWidth, imgWidth, ITEM_RXHD);
	}

	//学校区域
	if (m_nStartXmStatus & ITEM_XXQY_FLAG)
	{
		DrawEnterItem(graphics, x + 10 * splitWidth, y, imgWidth, imgWidth, ITEM_XXQY);
	}

	//公交车站
	if (m_nStartXmStatus & ITEM_GJCZ_FLAG)
	{
		DrawEnterItem(graphics, x + 11 * splitWidth, y, imgWidth, imgWidth, ITEM_GJCZ);
	}

	//会车
	if (m_nStartXmStatus & ITEM_HC_FLAG)
	{
		DrawEnterItem(graphics, x + 12 * splitWidth, y, imgWidth, imgWidth, ITEM_HC);
	}

	//超车
	if (m_nStartXmStatus & ITEM_CC_FLAG)
	{
		DrawEnterItem(graphics, x + 13 * splitWidth, y, imgWidth, imgWidth, ITEM_CC);
	}

	//掉头
	if (m_nStartXmStatus & ITEM_DT_FLAG)
	{
		DrawEnterItem(graphics, x + 14 * splitWidth, y, imgWidth, imgWidth, ITEM_DT);
	}

	//夜间
	if (m_nStartXmStatus & ITEM_YJ_FLAG)
	{
		DrawEnterItem(graphics, x + 15 * splitWidth, y, imgWidth, imgWidth, ITEM_YJ);
	}
}

void CStudentInfoReflesh::DrawKM3LeaveItem(Graphics *graphics)
{
	int imgWidth = 60;
	int splitWidth = DISPLAY_CHAN_WIDTH / 3 * 2 / ItemCount::ITEM_COUNT_16;
	int x = (splitWidth - imgWidth) / 2;
	int y = 295;

	//上车准备
	if (m_nEndXmStatus & ITEM_SCZB_FLAG)
	{
		DrawLeaveItem(graphics, x + 0 * splitWidth, y, imgWidth, imgWidth, ITEM_SCZB);
	}

	//起步
	if (m_nEndXmStatus & ITEM_QB_FLAG)
	{
		DrawLeaveItem(graphics, x + 1 * splitWidth, y, imgWidth, imgWidth, ITEM_QB);
	}

	//直线行驶
	if (m_nEndXmStatus & ITEM_ZXXS_FLAG)
	{
		DrawLeaveItem(graphics, x + 2 * splitWidth, y, imgWidth, imgWidth, ITEM_ZXXS);
	}

	//加减档
	if (m_nEndXmStatus & ITEM_JJD_FLAG)
	{
		DrawLeaveItem(graphics, x + 3 * splitWidth, y, imgWidth, imgWidth, ITEM_JJD);
	}

	//变更车道
	if (m_nEndXmStatus & ITEM_BGCD_FLAG)
	{
		DrawLeaveItem(graphics, x + 4 * splitWidth, y, imgWidth, imgWidth, ITEM_BGCD);
	}

	//靠边停车
	if (m_nEndXmStatus & ITEM_KBTC_FLAG)
	{
		DrawLeaveItem(graphics, x + 5 * splitWidth, y, imgWidth, imgWidth, ITEM_KBTC);
	}

	//直行通过路口
	if (m_nEndXmStatus & ITEM_ZHIXING_FLAG)
	{
		DrawLeaveItem(graphics, x + 6 * splitWidth, y, imgWidth, imgWidth, ITEM_ZX);
	}

	//左转
	if (m_nEndXmStatus & ITEM_ZZ_FLAG)
	{
		DrawLeaveItem(graphics, x + 7 * splitWidth, y, imgWidth, imgWidth, ITEM_ZZ);
	}

	//右转
	if (m_nEndXmStatus & ITEM_YZ_FLAG)
	{
		DrawLeaveItem(graphics, x + 8 * splitWidth, y, imgWidth, imgWidth, ITEM_YZ);
	}

	//人行横道
	if (m_nEndXmStatus & ITEM_RXHD_FLAG)
	{
		DrawLeaveItem(graphics, x + 9 * splitWidth, y, imgWidth, imgWidth, ITEM_RXHD);
	}

	//学校区域
	if (m_nEndXmStatus & ITEM_XXQY_FLAG)
	{
		DrawLeaveItem(graphics, x + 10 * splitWidth, y, imgWidth, imgWidth, ITEM_XXQY);
	}

	//公交车站
	if (m_nEndXmStatus & ITEM_GJCZ_FLAG)
	{
		DrawLeaveItem(graphics, x + 11 * splitWidth, y, imgWidth, imgWidth, ITEM_GJCZ);
	}

	//会车
	if (m_nEndXmStatus & ITEM_HC_FLAG)
	{
		DrawLeaveItem(graphics, x + 12 * splitWidth, y, imgWidth, imgWidth, ITEM_HC);
	}

	//超车
	if (m_nEndXmStatus & ITEM_CC_FLAG)
	{
		DrawLeaveItem(graphics, x + 13 * splitWidth, y, imgWidth, imgWidth, ITEM_CC);
	}

	//掉头
	if (m_nEndXmStatus & ITEM_DT_FLAG)
	{
		DrawLeaveItem(graphics, x + 14 * splitWidth, y, imgWidth, imgWidth, ITEM_DT);
	}

	//夜间
	if (m_nEndXmStatus & ITEM_YJ_FLAG)
	{
		DrawLeaveItem(graphics, x + 15 * splitWidth, y, imgWidth, imgWidth, ITEM_YJ);
	}
}

void CStudentInfoReflesh::DrawKM2EnterItem(Graphics *graphics)
{
	int imgWidth = 60;
	int splitWidth = DISPLAY_CHAN_WIDTH / 3 * 2 / m_nKM2ItemCount;
	int x = (splitWidth - imgWidth) / 2;
	int y = 295;

	//大部分地方科目二都是5个项目
	if (ItemCount::ITEM_COUNT_5 == m_nKM2ItemCount || ItemCount::ITEM_COUNT_7 == m_nKM2ItemCount)
	{
		//倒车入库
		if (m_nStartXmStatus & ITEM_DCRK_FLAG)
		{
			DrawEnterItem(graphics, x + 0 * splitWidth, y, imgWidth, imgWidth, ITEM_DCRK);
		}
		//侧方停车
		if (m_nStartXmStatus & ITEM_CFTC_FLAG)
		{
			DrawEnterItem(graphics, x + 1 * splitWidth, y, imgWidth, imgWidth, ITEM_CFTC);
		}
		//半坡起步
		if (m_nStartXmStatus & ITEM_BPQB_FLAG)
		{
			DrawEnterItem(graphics, x + 2 * splitWidth, y, imgWidth, imgWidth, ITEM_BPQB);
		}
		//曲线行驶
		if (m_nStartXmStatus & ITEM_QXXS_FLAG)
		{
			DrawEnterItem(graphics, x + 3 * splitWidth, y, imgWidth, imgWidth, ITEM_QXXS);
		}
		//直角转弯
		if (m_nStartXmStatus & ITEM_ZJZW_FLAG)
		{
			DrawEnterItem(graphics, x + 4 * splitWidth, y, imgWidth, imgWidth, ITEM_ZJZW);
		}

		if (ItemCount::ITEM_COUNT_7 == m_nKM2ItemCount)
		{
			//雨雾湿滑
			if (m_nStartXmStatus & ITEM_YWSH_FLAG)
			{
				DrawEnterItem(graphics, x + 5 * splitWidth, y, imgWidth, imgWidth, ITEM_YWSH);
			}
			//模拟隧道
			if (m_nStartXmStatus & ITEM_MNSD_FLAG)
			{
				DrawEnterItem(graphics, x + 6 * splitWidth, y, imgWidth, imgWidth, ITEM_MNSD);
			}
		}
	}
}

void CStudentInfoReflesh::DrawKM2LeaveItem(Graphics *graphics)
{
	int imgWidth = 60;
	int splitWidth = DISPLAY_CHAN_WIDTH / 3 * 2 / m_nKM2ItemCount;
	int x = (splitWidth - imgWidth) / 2;
	int y = 295;

	//大部分地方科目二都是5个项目
	if (ItemCount::ITEM_COUNT_5 == m_nKM2ItemCount || ItemCount::ITEM_COUNT_7 == m_nKM2ItemCount)
	{
		//倒车入库
		if (m_nEndXmStatus & ITEM_DCRK_FLAG)
		{
			DrawLeaveItem(graphics, x + 0 * splitWidth, y, imgWidth, imgWidth, ITEM_DCRK);
		}
		//侧方停车
		if (m_nEndXmStatus & ITEM_CFTC_FLAG)
		{
			DrawLeaveItem(graphics, x + 1 * splitWidth, y, imgWidth, imgWidth, ITEM_CFTC);
		}
		//半坡起步
		if (m_nEndXmStatus & ITEM_BPQB_FLAG)
		{
			DrawLeaveItem(graphics, x + 2 * splitWidth, y, imgWidth, imgWidth, ITEM_BPQB);
		}
		//曲线行驶
		if (m_nEndXmStatus & ITEM_QXXS_FLAG)
		{
			DrawLeaveItem(graphics, x + 3 * splitWidth, y, imgWidth, imgWidth, ITEM_QXXS);
		}
		//直角转弯
		if (m_nEndXmStatus & ITEM_ZJZW_FLAG)
		{
			DrawLeaveItem(graphics, x + 4 * splitWidth, y, imgWidth, imgWidth, ITEM_ZJZW);
		}

		if (ItemCount::ITEM_COUNT_7 == m_nKM2ItemCount)
		{
			//雨雾湿滑
			if (m_nEndXmStatus & ITEM_YWSH_FLAG)
			{
				DrawLeaveItem(graphics, x + 5 * splitWidth, y, imgWidth, imgWidth, ITEM_YWSH);
			}
			//模拟隧道
			if (m_nEndXmStatus & ITEM_MNSD_FLAG)
			{
				DrawLeaveItem(graphics, x + 6 * splitWidth, y, imgWidth, imgWidth, ITEM_MNSD);
			}
		}
	}
}

void CStudentInfoReflesh::DrawResult(Graphics *graphics)
{
	try
	{
		//档位信号
		wstring wsResultPath = m_wsProgramPath + IMG_PATH_RESULT;
		if (!CWinUtils::FileExists(wsResultPath))
		{
			L_ERROR(_T("file not exist : %s\n"), wsResultPath.c_str());
		}
		Image *imgResult = Image::FromFile(wsResultPath.c_str());

		int x = 80;
		int y = 50;
		int width = 220;
		int height = 200;

		int imgWidth = imgResult->GetWidth();
		int imgHeight = imgResult->GetHeight();

		if (m_bPass)
		{
			graphics->DrawImage(imgResult, Rect(x, y, width, height), 0, 0, imgWidth / 2, imgHeight, UnitPixel);
		}
		else
		{
			graphics->DrawImage(imgResult, Rect(x, y, width, height), imgWidth / 2, 0, imgWidth / 2, imgHeight, UnitPixel);
		}

		delete imgResult;
	}
	catch (...)
	{
		L_ERROR(_T("DrawResult catch an error.\n"));
	}
}