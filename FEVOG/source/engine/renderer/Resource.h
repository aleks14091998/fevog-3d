#ifndef _I_RESOURCE_H
#define _I_RESOURCE_H

class IResource {
private:
	char* name;
public:
	inline char* GetName() {
		return name;
	}

	inline void SetName(char* NewName) {
		name = NewName;
	}
};

#endif
