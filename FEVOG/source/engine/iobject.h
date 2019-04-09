#ifndef _I_OBJECT_H
#define _I_OBJECT_H

class IObject {
private:
	char *Name = (char*)"Object";
public:
	inline void SetName(char *name) {
		Name = name;
	}
	inline char* GetName() {
		return Name;
	}
	virtual void init() = 0;
	virtual void process() = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
};

#endif