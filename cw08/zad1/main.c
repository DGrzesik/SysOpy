#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>

//    file format:

//    P2   - "magic number"
//    # [...].ascii.pgm - file name
//    W H  -  width height
//    M    -  max pixel value
//    ...

int** image;
int** negative_image;
int width, height;
int threads_number;


void prepare_image(char* filename){
    FILE* image_file = fopen(filename, "r");
    if (image_file == NULL){
        printf("Error while opening input image file!\n");
        exit(1);
    }

    char* buffer = calloc(256, sizeof(char));

    // skip "magic number" (P2)
    fgets(buffer, 256, image_file);

    bool M_read;
    bool image_read = false;

    while(!image_read && fgets(buffer, 256, image_file)){
        if (buffer[0] == '#') continue;

        else if (!M_read){
            sscanf(buffer, "%d %d\n", &width, &height);
            printf("w: %d, h: %d\n", width, height);
            M_read = true;
        }

        else {
            int M;
            sscanf(buffer, "%d \n", &M);
            printf("M: %d\n", M);
            if (M != 255){
                printf("Max pixel value must equal 255!\n");
                exit(1);
            }
            image_read = true;
        }
    }

    image = calloc(height, sizeof(int *)); //creating matrix with the dimensions of the original image
    for (int i = 0; i < height; i++) {
        image[i] = calloc(width, sizeof(int));
    }

    int pixel;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            fscanf(image_file, "%d", &pixel);
            image[i][j] = pixel;
        }
    }

    fclose(image_file);
}

void create_negative(char* filename) {
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Error while opening output image file!\n");
        exit(1);
    }

    fprintf(file, "P2\n");
    fprintf(file, "%d %d\n", width, height);
    fprintf(file, "255\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fprintf(file, "%d ", negative_image[i][j]);//filling image file with values from negative_image matrix
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void* numbers_method(void* arg){
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    int t_id = *((int *) arg);
    // pixels value range for thread: [256/n*i, 256/n*(i+1)], where n is the number of threads and i is the thread's id
    // last thread works on remaining pixels
    int from = 256 / threads_number * t_id;
    int to;
    if (t_id!=threads_number-1){
        to=(256 / threads_number * (t_id + 1) );
    }
    else{
        to=256;
    }
    int pixel;
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            pixel = image[i][j];
            if (pixel >= from && pixel < to){
                 negative_image[i][j] = 255 - pixel;

            }
        }
    }
    gettimeofday(&stop, NULL);
    long unsigned int* t = malloc(sizeof(long unsigned int));
    *t = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(t);
}


void* block_method(void* arg) {
    struct timeval stop, start;
    gettimeofday(&start, NULL);

    int t_id = *((int *) arg);
    int x_from = (t_id) * ceil((width / threads_number)); // thread numbers, k, start from 0 so I had to change the formula
    int x_to;
    if (t_id != threads_number - 1){
        x_to=((t_id + 1)* ceil(width / threads_number) - 1);
    }
    else{
        x_to=width - 1;
    }


    int pixel;
    for (int i = 0; i < height; i++){
        for (int j = x_from; j <= x_to; j++){
            pixel = image[i][j];
            negative_image[i][j] = 255 - pixel;
        }
    }

    gettimeofday(&stop, NULL);
    long unsigned int* t = malloc(sizeof(long unsigned int));
    *t = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    pthread_exit(t);
}


int main(int argc, char* argv[]){

    if (argc != 5){
        printf("Wrong number of arguments!\n");
        exit(1);
    }

    threads_number = atoi(argv[1]);
    char* method = argv[2];
    char* input_file = argv[3];
    char* output_file = argv[4];

    if ((strcmp(method, "numbers") != 0) && (strcmp(method, "block") != 0)) {
        printf("This method doesn't exist!\n");
        exit(1);
    }

    prepare_image(input_file);

    negative_image = calloc(height, sizeof(int *)); //creating matrix with the dimensions of the original image
    for (int i = 0; i < height; i++) {
        negative_image[i] = calloc(width, sizeof(int));
    }

    pthread_t* threads = calloc(threads_number, sizeof(pthread_t)); //creating table of threads
    int* threads_idx = calloc(threads_number, sizeof(int)); // creating table of thread indexes

    struct timeval stop, start;
    gettimeofday(&start, NULL);

    for(int i = 0; i < threads_number; i++){
        threads_idx[i] = i;
        // using thread index as an argument so it is possible to evenly distribute image parts to threads

        if (strcmp(method, "numbers") == 0){
            pthread_create(&threads[i], NULL, &numbers_method, &threads_idx[i]);
            sleep(2);
        }

        else if (strcmp(method, "block") == 0){
            pthread_create(&threads[i], NULL, &block_method, &threads_idx[i]);
        }
    }
    FILE* times_file = fopen("times.txt", "a");
    printf("Number of threads:  %d\n", threads_number);
    printf("Method:  %s\n", method);
    printf("Image size: %dx%d\n\n\n",width,height);
    fprintf(times_file, "Number of threads:  %d\n", threads_number);
    fprintf(times_file, "Method:  %s\n\n\n", method);
    fprintf(times_file,"Image size: %dx%d\n\n\n",width,height);

    for(int i = 0; i < threads_number; i++) {
        long unsigned int* t;
        pthread_join(threads[i], (void **) &t);
        printf("Thread index: %3d, work time: %5lu [μs]\n", i, *t);
        fprintf(times_file, "Thread index: %3d, work time: %5lu [μs]\n", i, *t);
    }

    gettimeofday(&stop, NULL);
    long unsigned int* t = malloc(sizeof(long unsigned int));
    *t = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("TOTAL TIME: %5lu [μs]\n", *t);
    fprintf(times_file, "TOTAL TIME: %5lu [μs]\n\n\n", *t);
    printf("--------------------\n\n");
    fprintf(times_file,"--------------------\n\n");
    create_negative(output_file);

    fclose(times_file);

    for (int i = 0; i < height; i++) {
        free(image[i]);
        free(negative_image[i]);
    }
    free(image);
    free(negative_image);

    return 0;
}