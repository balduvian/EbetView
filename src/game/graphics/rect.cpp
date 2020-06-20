
#include <GL/glew.h>

#include "rect.h"
#include "../../cnge8/types.h"

namespace Game {
	Rect::Rect() : VAO(
		GL_TRIANGLES,
		CNGE::Attribute::createVertexAttribute(12, new f32[12] {
			0, 0, 0,
			0, 1, 0,
			1, 1, 0,
			1, 0, 0
		}),
		6, new u32[6] {
			0, 3, 2,
			0, 2, 1
		},
		1, new CNGE::Attribute[1] {
			CNGE::Attribute(2, 8, new f32[8] {
				0, 0,
				0, 1,
				1, 1,
				1, 0
			})
		}
	) {}
}
