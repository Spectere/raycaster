#include "defs.h"
#include "game.h"
#include "video.h"

int main() {
	if(video_init("Raycaster", RENDER_WIDTH, RENDER_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT)) {
		return 1;
	}

	game_loop();
	video_close();

	return 0;
}
