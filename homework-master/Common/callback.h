#ifndef _CALL_BACK_
#define _CALL_BACK_

/************************************************************************
   created: 2018/01/17
   class: 
   author: linyi
   usage: ����࣬���������̲���
************************************************************************/

class ICallbacks
{
public:
	ICallbacks(){};
	// ������
	virtual void MouseAction(double x, double y){};
	// ���̲���
	virtual void KeyAction(int key,int action){};
	// ��Ⱦ����
	virtual void RenderScene(){};
	virtual ~ICallbacks(){};
};

#endif