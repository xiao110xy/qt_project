#ifndef   pixel_annotation_tool_version_h
#define   pixel_annotation_tool_version_h

# ifdef __cplusplus
extern "C" {
# endif

#define PIXEL_ANNOTATION_TOOL_ARCH            "x64" /*! Current archi x86 or x64 */
#define PIXEL_ANNOTATION_TOOL_GIT_BRANCH      "" /*! Current name of the branch */
#define PIXEL_ANNOTATION_TOOL_GIT_COMMIT_HASH "" /*! Current hash of the git branch */
#define PIXEL_ANNOTATION_TOOL_GIT_DATE		     "" /*! Current date of the git branch */
#define PIXEL_ANNOTATION_TOOL_GIT_TAG	       "" /*! Current date of the git branch */

#if _DEBUG
#define PIXEL_ANNOTATION_TOOL_CONF_MODE					"DEBUG-x64" /*! Last compilation mode used */
#else
#define PIXEL_ANNOTATION_TOOL_CONF_MODE					"RELEASE-x64"/*! Last compilation mode used */
#endif
#define INFO_PIXEL_ANNOTATION_TOOL_VERSION			" -  -  -  - " PIXEL_ANNOTATION_TOOL_CONF_MODE /*! Version name */

# ifdef __cplusplus
}
# endif

#endif // pixel_annotation_tool_version_h
