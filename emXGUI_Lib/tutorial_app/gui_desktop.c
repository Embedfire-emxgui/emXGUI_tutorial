/**
  *********************************************************************
  * @file    desktop.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   emXGUI ����
  *********************************************************************
  * ʵ��ƽ̨:Ұ�� F429-��ս�� STM32 ������
  * ����    :www.embedfire.com
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  **********************************************************************
  */ 


#include	"rtthread.h"
#include "emXGUI.h"





/*===================================================================================*/
extern void GUI_AppMain(void);


static	void	gui_app_thread(void *p)
{
    #if(GUI_TOUCHSCREEN_EN & GUI_TOUCHSCREEN_CALIBRATE)
    {
        int i=0;
        while(TouchPanel_IsPenDown())
    	{
    		GUI_msleep(100);
    		if(i++>10)
    		{
    			ShowCursor(FALSE);
    			TouchScreenCalibrate(NULL);
    			ShowCursor(TRUE);
    			break;
    		}
    	}
    }
    #endif
		
  /* ����APP���� */  
//	GUI_AppMain();
 //   GUI_UserAppStart();
//   	ShellWindowStartup();
  //  return 0;
}

/*===================================================================================*/

/**
  * @brief  ���汳�����ƺ�����������������ʱ����ñ�������
            ͨ���޸ı����������ݿɸ�������ı���
  * @param  hdc ��ͼ������
  * @param  lprc Ҫ���Ƶľ�������ΪNULLʱ���Զ�����hwnd�Ŀͻ���
  * @param  hwnd ���ڶ�����
  * @retval ��
  */
static	void	_EraseBackgnd(HDC hdc,const RECT *lprc,HWND hwnd)
{
	RECT rc;

	if(lprc==NULL)
	{
		GetClientRect(hwnd,&rc);
	}
	else
	{
		CopyRect(&rc,lprc);
	}

	SetBrushColor(hdc,MapRGB(hdc,32,72,144));
	FillRect(hdc,&rc);
  	
  SetTextColor(hdc,MapRGB(hdc,250,250,250));

  /* ������ʾ��� */
	DrawText(hdc,L"Hello emXGUI@Embedfire!",-1,&rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);

  
	SetTextColor(hdc,MapRGB(hdc,250,250,250));
	TextOut(hdc,20,20,L"emXGUI@Embedfire STM32F429 ",-1);

}

#if 0
static	int	gui_input_thread(void *p)
{
	SYS_thread_set_priority(NULL,+4);
	while(1)
	{
		GUI_InputHandler(); //���������豸
		GUI_msleep(20);
	}
}
#endif


/**
  * @brief  ����ص�����
  * @param  hwnd ��ǰ��������Ϣ�Ĵ��ڶ�����
  * @param  msg ��Ϣ����ֵ�����Ա�ʶ�����ֵ�ǰ��������Ϣ
  * @param  wParam ��Ϣ����ֵ������msg��Ϣ����ֵ��ͬ
  * @param  lParam ��Ϣ����ֵ������msg��Ϣ����ֵ��ͬ
  * @retval ���ظ�SendMessage��ֵ
  */
static 	 LRESULT  	desktop_proc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
    /* ���洴��ʱ,���������Ϣ,������������һЩ��ʼ������. */
		case	WM_CREATE:	


			   ////����1��20ms��ʱ��������ѭ���¼�.
				 SetTimer(hwnd,1,20,TMR_START,NULL);

				//����App�߳�						
				if(1)
				{
						rt_thread_t h;
					
						h=rt_thread_create("GUI_APP",gui_app_thread,NULL,2048,5,5);
						rt_thread_startup(h);				
				}

				break;

		/* ��ʱ���������豸����Ϣ */
		case	WM_TIMER:
      #if(GUI_INPUT_DEV_EN)
        {
          u16 id;

          id =LOWORD(wParam);
          if(id==1)
          {
            GUI_InputHandler(); //���������豸
          }
        }
      #endif
		break;

    /* �ͻ���������Ҫ������ */
		case	WM_ERASEBKGND:
		{
			HDC hdc =(HDC)wParam;
			_EraseBackgnd(hdc,NULL,hwnd);
		}
		return TRUE;

    /* �û������ĵ���Ϣ����ϵͳ���� */
		default:
				return	DefDesktopProc(hwnd,msg,wParam,lParam);
	}

	return WM_NULL;

}

/**
  * @brief  ������������
  * @param  ��
  * @retval ��
  */
void GUI_DesktopStartup(void)
{
	WNDCLASS	wcex;
	HWND hwnd;
	MSG msg;

	wcex.Tag 		    = WNDCLASS_TAG;

	wcex.Style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= desktop_proc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= NULL;//hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;//LoadCursor(NULL, IDC_ARROW);

	//�������洰��.
	hwnd = GUI_CreateDesktop(	WS_EX_LOCKPOS,
                              &wcex,
                              L"DESKTOP",
                              WS_VISIBLE|WS_CLIPCHILDREN,
                              0,0,GUI_XSIZE,GUI_YSIZE,
                              NULL,0,NULL,NULL);

	GUI_Printf("HWND_Desktop=%08XH\r\n",	hwnd);

	//��ʾ���洰��.
	ShowWindow(hwnd,SW_SHOW);

	//����ϵͳ�򿪹����ʾ(���԰�ʵ��������Ƿ���Ҫ).
	ShowCursor(TRUE);

	while(GetMessage(&msg,hwnd))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

/********************************END OF FILE****************************/
