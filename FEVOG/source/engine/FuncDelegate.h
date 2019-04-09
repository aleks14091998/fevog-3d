#ifndef _FUNCDEL_H
#define _FUNCDEL_H

#include"FEV_Eng.h"
#include<vector>
// add index
typedef void	(CALLBACK *TPProc)(void *pParametr),
				(CALLBACK *TPMsgProc)(void *pParametr, const valk::TWinMessage &stMsg);

template<class T1, class T2>
class FuncDelegate {
private:
	bool _bAllowInvoke;

	struct delItem {
		void *parametr;
		T1 func;
	};

	std::vector<delItem> funcList;

public:
	FuncDelegate() :_bAllowInvoke(true) {

	}

	void SetAllowInvoke(bool bAllow) {
		_bAllowInvoke = bAllow;
	}

	inline bool IsNull() {
		return funcList.empty();
	}

	void addFunc(T1 func, void *parametr) {
		delItem item;
		item.parametr = parametr;
		item.func = func;
		funcList.push_back(item);
	}

	void removeFunc(T1 func, void *parametr) {
		for (std::size_t i = 0; i < funcList.size(); i++) {
			if (funcList[i].parametr == parametr && funcList[i].func == func) {
				funcList.erase(funcList.begin() + i);
				break;
			}
		}
	}

//	T2 Invoke;
	void Invoke();
	void Invoke(const valk::TWinMessage &stMsg);
};

inline void FuncDelegate<TPProc, void()>::Invoke() {
	if (_bAllowInvoke) {
		for (std::size_t i = 0; i < funcList.size(); i++) {
			(*funcList[i].func)(funcList[i].parametr);
		}
	}
}

inline void FuncDelegate<TPMsgProc, void(const valk::TWinMessage &)>::Invoke(const valk::TWinMessage &stMsg) {
	if (_bAllowInvoke) {
		for (std::size_t i = 0; i < funcList.size(); i++) {
			(*funcList[i].func)(funcList[i].parametr, stMsg);
		}
	}
}

typedef FuncDelegate<TPProc, void()> ProcDelegate;
typedef FuncDelegate<TPMsgProc, void(const valk::TWinMessage &)> MsgProcDelegate;

#endif //_FUNCDEL_H