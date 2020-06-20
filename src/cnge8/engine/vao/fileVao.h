
#ifndef CNGE_FILE_VAO
#define CNGE_FILE_VAO

#include "../vao.h"

namespace CNGE {
	class FileVAO : public VAO {
	private:
		const char* filePath;

	protected:
		void customGather();
		void customDiscard();

	public:
		FileVAO(const char*);

	};
}

#endif

