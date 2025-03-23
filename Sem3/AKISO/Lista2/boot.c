#define VGA_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define MAX_COLORS 16

void main() {
    char *video_memory = (char*) VGA_MEMORY;
    char *message = "Hello, World!";
    int position = 0; 
    for (int fg = 0; fg < MAX_COLORS; fg++) {
        position = fg * SCREEN_WIDTH * 2; 
        
        for (int i = 0; message[i] != '\0'; i++) {
            video_memory[position + i * 2] = message[i];           
            video_memory[position + i * 2 + 1] = (0 << 4) | fg;    
        }
    }
    for (int bg = 0; bg < MAX_COLORS; bg++) {
 
       position = bg * SCREEN_WIDTH * 2+132;
        
        for (int i = 0; message[i] != '\0'; i++) {
            video_memory[position + i * 2] = message[i];           
            video_memory[position + i * 2 + 1] =(bg<<4)| 0;   
        }
    }

    while(1); 
}
