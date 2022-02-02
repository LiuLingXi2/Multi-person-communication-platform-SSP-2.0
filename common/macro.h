#ifndef __MACRO_H__

#define __MACRO_H__

#define GETSETVAR(type, name) \
private: \
	type name##_; \
public: \
	const type& name() const { \
		return name##_; \
	} \
	void set_##name(const type& val){ \
		name##_ = val; \
	}

#define GETSETSTR(size, name) \
private: \
	char name##_[size]; \
public: \
	const char* name() const { \
		return name##_; \
	} \
	void set_##name(const char * val) { \
		strncpy(name##_,val,256); \
	}

#endif // !__MACRO_H__
