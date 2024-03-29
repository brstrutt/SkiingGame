#ifndef __SHADERBASE_H__
#define __SHADERBASE_H__


#define INVALID_SHADER_HANDLE			0xffffffff
#define INVALID_SHADER_PROGRAM_HANDLE	0xffffffff
#define MAX_NUM_SHADER_PER_PROGRAM		4

typedef unsigned int ShaderHandle;
typedef unsigned int ShaderProgramHandle;

enum EShaderType
{
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_UNDEFINED
};

enum EShaderError
{
	SHADER_ERROR_NONE = 0,
	SHADER_ERROR_CREATE,
	SHADER_ERROR_COMPILE
};

typedef struct _ShaderInternal
{
	char _name[17];		//name , no more than 16 characters please.
	EShaderType _type;
	ShaderHandle _handle;		//Application assigned handle; should be assigned by the manager.
	ShaderHandle _syshandle;		//System assigned handle, generated by the graphics API. NOTE never ever ever ever assign this manually.
} ShaderInternal;

class ShaderBase
{

protected:
	ShaderInternal m_shaderInternal;

public:
	ShaderBase()
	{
		m_shaderInternal._name[0] = '\0';
		m_shaderInternal._type = SHADER_UNDEFINED;
		m_shaderInternal._handle = INVALID_SHADER_HANDLE;
		m_shaderInternal._syshandle = INVALID_SHADER_HANDLE;
	}
	virtual ~ShaderBase(){;}

	inline void SetShaderType(EShaderType type)
	{
		m_shaderInternal._type = type;
	}

	inline EShaderType GetShaderType()
	{
		return m_shaderInternal._type;
	}

	inline void SetShaderHandle(ShaderHandle handle)
	{
		m_shaderInternal._handle = handle;
	}

	inline ShaderHandle GetShaderHandle()
	{
		return m_shaderInternal._handle;
	}

	inline ShaderHandle GetShaderSysHandle()
	{
		return m_shaderInternal._syshandle;
	}

	virtual void SetShaderOwnerProgram(ShaderProgramHandle handle) = 0;

	virtual EShaderError CreateShaderFromSourceFile(LPCWSTR filename, EShaderType type) = 0;
	virtual void DeleteShader() = 0;
};

#endif
