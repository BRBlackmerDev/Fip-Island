
#ifdef	STD_INCLUDE_DECL

typedef uint32 ( CREATE_PROGRAM )();
typedef uint32 ( CREATE_SHADER )( GLenum shaderType );
typedef void ( SHADER_SOURCE )( GLuint shader, GLsizei count, const GLchar ** string, const GLint * length );
typedef void ( COMPILE_SHADER )( GLuint shader );
typedef void ( GET_SHADERIV )( GLuint shader, GLenum pname, GLint * params );
typedef void ( GET_SHADER_INFO_LOG )( GLuint shader, GLsizei maxLength, GLsizei * length, GLchar * infoLog );
typedef void ( ATTACH_SHADER )( GLuint program, GLuint shader );
typedef void ( LINK_PROGRAM )( GLuint program );
typedef void ( GET_PROGRAMIV )( GLuint program, GLenum pname, GLint * params );
typedef void ( GET_PROGRAM_INFO_LOG )( GLuint program, GLsizei maxLength, GLsizei * length, GLchar * infoLog );
typedef void ( GEN_BUFFERS )( GLsizei n, GLuint * buffers );
typedef void ( DELETE_BUFFERS )( GLsizei n, const GLuint * buffers );
typedef void ( BUFFER_DATA )( GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage );
typedef void ( BIND_BUFFER )( GLenum target, GLuint buffer );
typedef void ( VERTEX_ATTRIB_POINTER )( GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer );
typedef void ( ENABLE_VERTEX_ATTRIB_ARRAY )( GLuint index );
typedef void ( DISABLE_VERTEX_ATTRIB_ARRAY )( GLuint index );
typedef void ( UNIFORM1I )( GLint location, GLint v0 );
typedef void ( UNIFORM1F )( GLint location, GLfloat v0 );
typedef void ( UNIFORM3F )( GLint location, GLfloat v0, GLfloat v1, GLfloat v2 );
typedef void ( UNIFORM4F )( GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3 );
typedef void ( UNIFORM2FV )( GLint location, GLsizei count, const GLfloat * value );
typedef void ( UNIFORM3FV )( GLint location, GLsizei count, const GLfloat * value );
typedef void ( UNIFORM4FV )( GLint location, GLsizei count, const GLfloat * value );
typedef void ( USE_PROGRAM )( GLuint program );
typedef GLint ( GET_ATTRIB_LOCATION )( GLuint program, const GLchar * name );
typedef GLint ( GET_UNIFORM_LOCATION )( GLuint program, const GLchar * name );
typedef void ( UNIFORM_MATRIX4F )( GLint location, GLsizei count, GLboolean transpose, const GLfloat * value );
typedef void ( ACTIVE_TEXTURE )( GLenum texture );
typedef void ( BIND_BUFFER_RANGE )( GLenum target, GLuint index, GLuint buffer, GLint ptroffset, GLsizei ptrsize );
typedef GLuint ( GET_UNIFORM_BLOCK_INDEX )( GLuint program, const GLchar * uniformBlockName );
typedef void ( UNIFORM_BLOCK_BINDING )( GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding );
typedef void ( BUFFER_SUB_DATA )( GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data );
typedef void ( BIND_BUFFER_BASE )( GLenum target, GLuint index, GLuint buffer );
typedef void ( DELETE_SHADER )( GLuint shader );
typedef void ( GEN_FRAMEBUFFERS )( GLsizei n, GLuint * framebuffers );
typedef void ( BIND_FRAMEBUFFER )( GLenum target, GLuint framebuffer );
typedef void ( FRAMEBUFFER_TEXTURE )( GLenum target, GLenum attachment, GLuint texture, GLint level );
typedef void ( FRAMEBUFFER_TEXTURE2D )( GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level );
typedef GLenum ( CHECK_FRAMEBUFFER_STATUS )( GLenum target );
typedef void ( GEN_RENDERBUFFERS )( GLsizei n, GLuint * renderbuffers );
typedef void ( BIND_RENDERBUFFER )( GLenum target, GLuint renderbuffer );
typedef void ( RENDERBUFFER_STORAGE )( GLenum target, GLenum internalformat, GLsizei width, GLsizei height );
typedef void ( FRAMEBUFFER_RENDERBUFFER )( GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer );
typedef void ( CLEAR_BUFFER )( GLenum buffer, GLint drawbuffer, const GLfloat * value );
typedef void ( FRAMEBUFFER_TEXTURE_LAYER )( GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer );
typedef void ( GENERATE_MIPMAP )( GLenum target );
typedef GLboolean ( IS_BUFFER )( GLuint buffer );
typedef void ( MULTI_DRAW_ARRAYS_INDIRECT )( GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride );
typedef void ( VERTEX_ATTRIB_DIVISOR )( GLuint index, GLuint divisor );
typedef void * ( MAP_BUFFER )( GLenum target, GLenum access );
typedef GLboolean ( UNMAP_BUFFER )( GLenum target );
typedef GLenum ( CLIENT_WAIT_SYNC )( GLsync sync, GLbitfield flags, GLuint64 timeout );
typedef GLsync ( FENCE_SYNC )( GLenum condition, GLbitfield flags );
typedef void ( BUFFER_STORAGE )( GLenum target, GLsizeiptr size, const GLvoid * data, GLbitfield flags );
typedef void * ( MAP_BUFFER_RANGE )( GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access );

#endif	// STD_INCLUDE_DECL
#ifdef	STD_INCLUDE_FUNC

CREATE_PROGRAM * glCreateProgram = {};
CREATE_SHADER * glCreateShader = {};
SHADER_SOURCE * glShaderSource = {};
COMPILE_SHADER * glCompileShader = {};
GET_SHADERIV * glGetShaderiv = {};
GET_SHADER_INFO_LOG * glGetShaderInfoLog = {};
ATTACH_SHADER * glAttachShader = {};
LINK_PROGRAM * glLinkProgram = {};
GET_PROGRAMIV * glGetProgramiv = {};
GET_PROGRAM_INFO_LOG * glGetProgramInfoLog = {};
GEN_BUFFERS * glGenBuffers = {};
DELETE_BUFFERS * glDeleteBuffers = {};
BUFFER_DATA * glBufferData = {};
BIND_BUFFER * glBindBuffer = {};
VERTEX_ATTRIB_POINTER * glVertexAttribPointer = {};
ENABLE_VERTEX_ATTRIB_ARRAY * glEnableVertexAttribArray = {};
DISABLE_VERTEX_ATTRIB_ARRAY * glDisableVertexAttribArray = {};
UNIFORM1I * glUniform1i = {};
UNIFORM1F * glUniform1f = {};
UNIFORM3F * glUniform3f = {};
UNIFORM4F * glUniform4f = {};
UNIFORM2FV * glUniform2fv = {};
UNIFORM3FV * glUniform3fv = {};
UNIFORM4FV * glUniform4fv = {};
USE_PROGRAM * glUseProgram = {};
GET_ATTRIB_LOCATION * glGetAttribLocation = {};
GET_UNIFORM_LOCATION * glGetUniformLocation = {};
UNIFORM_MATRIX4F * glUniformMatrix4fv = {};
ACTIVE_TEXTURE * glActiveTexture = {};
BIND_BUFFER_RANGE * glBindBufferRange = {};
GET_UNIFORM_BLOCK_INDEX * glGetUniformBlockIndex = {};
UNIFORM_BLOCK_BINDING * glUniformBlockBinding = {};
BUFFER_SUB_DATA * glBufferSubData = {};
BIND_BUFFER_BASE * glBindBufferBase = {};
DELETE_SHADER * glDeleteShader = {};
GEN_FRAMEBUFFERS * glGenFramebuffers = {};
BIND_FRAMEBUFFER * glBindFramebuffer = {};
FRAMEBUFFER_TEXTURE * glFramebufferTexture = {};
FRAMEBUFFER_TEXTURE2D * glFramebufferTexture2D = {};
CHECK_FRAMEBUFFER_STATUS * glCheckFramebufferStatus = {};
GEN_RENDERBUFFERS * glGenRenderbuffers = {};
BIND_RENDERBUFFER * glBindRenderbuffer = {};
RENDERBUFFER_STORAGE * glRenderbufferStorage = {};
FRAMEBUFFER_RENDERBUFFER * glFramebufferRenderbuffer = {};
CLEAR_BUFFER * glClearBufferfv = {};
FRAMEBUFFER_TEXTURE_LAYER * glFramebufferTextureLayer = {};
GENERATE_MIPMAP * glGenerateMipmap = {};
IS_BUFFER * glIsBuffer = {};
MULTI_DRAW_ARRAYS_INDIRECT * glMultiDrawArraysIndirect = {};
VERTEX_ATTRIB_DIVISOR * glVertexAttribDivisor = {};
MAP_BUFFER * glMapBuffer = {};
UNMAP_BUFFER * glUnmapBuffer = {};
CLIENT_WAIT_SYNC * glClientWaitSync = {};
FENCE_SYNC * glFenceSync = {};
BUFFER_STORAGE * glBufferStorage = {};
MAP_BUFFER_RANGE * glMapBufferRange = {};

internal void
GL_win32_getExtensions() {
	glCreateProgram = ( CREATE_PROGRAM * )wglGetProcAddress( "glCreateProgram" );
	glCreateShader = ( CREATE_SHADER * )wglGetProcAddress( "glCreateShader" );
	glShaderSource = ( SHADER_SOURCE * )wglGetProcAddress( "glShaderSource" );
	glCompileShader = ( COMPILE_SHADER * )wglGetProcAddress( "glCompileShader" );
	glGetShaderiv = ( GET_SHADERIV * )wglGetProcAddress( "glGetShaderiv" );
	glGetShaderInfoLog = ( GET_SHADER_INFO_LOG * )wglGetProcAddress( "glGetShaderInfoLog" );
	glAttachShader = ( ATTACH_SHADER * )wglGetProcAddress( "glAttachShader" );
	glLinkProgram = ( LINK_PROGRAM * )wglGetProcAddress( "glLinkProgram" );
	glGetProgramiv = ( GET_PROGRAMIV * )wglGetProcAddress( "glGetProgramiv" );
	glGetProgramInfoLog = ( GET_PROGRAM_INFO_LOG * )wglGetProcAddress( "glGetProgramInfoLog" );
	glGenBuffers = ( GEN_BUFFERS * )wglGetProcAddress( "glGenBuffers" );
	glDeleteBuffers = ( DELETE_BUFFERS * )wglGetProcAddress( "glDeleteBuffers" );
	glBufferData = ( BUFFER_DATA * )wglGetProcAddress( "glBufferData" );
	glBindBuffer = ( BIND_BUFFER * )wglGetProcAddress( "glBindBuffer" );
	glVertexAttribPointer = ( VERTEX_ATTRIB_POINTER * )wglGetProcAddress( "glVertexAttribPointer" );
	glEnableVertexAttribArray = ( ENABLE_VERTEX_ATTRIB_ARRAY * )wglGetProcAddress( "glEnableVertexAttribArray" );
	glDisableVertexAttribArray = ( DISABLE_VERTEX_ATTRIB_ARRAY * )wglGetProcAddress( "glDisableVertexAttribArray" );
	glUniform1i = ( UNIFORM1I * )wglGetProcAddress( "glUniform1i" );
	glUniform1f = ( UNIFORM1F * )wglGetProcAddress( "glUniform1f" );
	glUniform3f = ( UNIFORM3F * )wglGetProcAddress( "glUniform3f" );
	glUniform4f = ( UNIFORM4F * )wglGetProcAddress( "glUniform4f" );
	glUniform2fv = ( UNIFORM2FV * )wglGetProcAddress( "glUniform2fv" );
	glUniform3fv = ( UNIFORM3FV * )wglGetProcAddress( "glUniform3fv" );
	glUniform4fv = ( UNIFORM4FV * )wglGetProcAddress( "glUniform4fv" );
	glUseProgram = ( USE_PROGRAM * )wglGetProcAddress( "glUseProgram" );
	glGetAttribLocation = ( GET_ATTRIB_LOCATION * )wglGetProcAddress( "glGetAttribLocation" );
	glGetUniformLocation = ( GET_UNIFORM_LOCATION * )wglGetProcAddress( "glGetUniformLocation" );
	glUniformMatrix4fv = ( UNIFORM_MATRIX4F * )wglGetProcAddress( "glUniformMatrix4fv" );
	glActiveTexture = ( ACTIVE_TEXTURE * )wglGetProcAddress( "glActiveTexture" );
	glBindBufferRange = ( BIND_BUFFER_RANGE * )wglGetProcAddress( "glBindBufferRange" );
	glGetUniformBlockIndex = ( GET_UNIFORM_BLOCK_INDEX * )wglGetProcAddress( "glGetUniformBlockIndex" );
	glUniformBlockBinding = ( UNIFORM_BLOCK_BINDING * )wglGetProcAddress( "glUniformBlockBinding" );
	glBufferSubData = ( BUFFER_SUB_DATA * )wglGetProcAddress( "glBufferSubData" );
	glBindBufferBase = ( BIND_BUFFER_BASE * )wglGetProcAddress( "glBindBufferBase" );
	glDeleteShader = ( DELETE_SHADER * )wglGetProcAddress( "glDeleteShader" );
	glGenFramebuffers = ( GEN_FRAMEBUFFERS * )wglGetProcAddress( "glGenFramebuffers" );
	glBindFramebuffer = ( BIND_FRAMEBUFFER * )wglGetProcAddress( "glBindFramebuffer" );
	glFramebufferTexture = ( FRAMEBUFFER_TEXTURE * )wglGetProcAddress( "glFramebufferTexture" );
	glFramebufferTexture2D = ( FRAMEBUFFER_TEXTURE2D * )wglGetProcAddress( "glFramebufferTexture2D" );
	glCheckFramebufferStatus = ( CHECK_FRAMEBUFFER_STATUS * )wglGetProcAddress( "glCheckFramebufferStatus" );
	glGenRenderbuffers = ( GEN_RENDERBUFFERS * )wglGetProcAddress( "glGenRenderbuffers" );
	glBindRenderbuffer = ( BIND_RENDERBUFFER * )wglGetProcAddress( "glBindRenderbuffer" );
	glRenderbufferStorage = ( RENDERBUFFER_STORAGE * )wglGetProcAddress( "glRenderbufferStorage" );
	glFramebufferRenderbuffer = ( FRAMEBUFFER_RENDERBUFFER * )wglGetProcAddress( "glFramebufferRenderbuffer" );
	glClearBufferfv = ( CLEAR_BUFFER * )wglGetProcAddress( "glClearBufferfv" );
	glFramebufferTextureLayer = ( FRAMEBUFFER_TEXTURE_LAYER * )wglGetProcAddress( "glFramebufferTextureLayer" );
	glGenerateMipmap = ( GENERATE_MIPMAP * )wglGetProcAddress( "glGenerateMipmap" );
	glIsBuffer = ( IS_BUFFER * )wglGetProcAddress( "glIsBuffer" );
	glMultiDrawArraysIndirect = ( MULTI_DRAW_ARRAYS_INDIRECT * )wglGetProcAddress( "glMultiDrawArraysIndirect" );
	glVertexAttribDivisor = ( VERTEX_ATTRIB_DIVISOR * )wglGetProcAddress( "glVertexAttribDivisor" );
	glMapBuffer = ( MAP_BUFFER * )wglGetProcAddress( "glMapBuffer" );
	glUnmapBuffer = ( UNMAP_BUFFER * )wglGetProcAddress( "glUnmapBuffer" );
	glClientWaitSync = ( CLIENT_WAIT_SYNC * )wglGetProcAddress( "glClientWaitSync" );
	glFenceSync = ( FENCE_SYNC * )wglGetProcAddress( "glFenceSync" );
	glBufferStorage = ( BUFFER_STORAGE * )wglGetProcAddress( "glBufferStorage" );
	glMapBufferRange = ( MAP_BUFFER_RANGE * )wglGetProcAddress( "glMapBufferRange" );
}

#endif	// STD_INCLUDE_FUNC