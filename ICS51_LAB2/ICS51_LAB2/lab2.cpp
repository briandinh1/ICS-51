// Remember to fill in your name and student ID below.
char *yourName = "Brian Dinh";
char *yourStudentID = "34546266";


// Part1) This function takes in a square sized grayscale image and applies thresholding on each pixel.
// Width and height of the image are equal to "dim".
void imageThresholding(unsigned char* image, int dim) {
	

	__asm {
		push ebx;
		push edi;
		push esi;

// YOUR CODE STARTS HERE

		mov eax, dim
		mul eax
		mov ecx, eax
		dec ecx // now ecx is dim^2 - 1 = size of the array

		mov ebx, image

	StartLoop:
		mov al, byte ptr[ebx][ecx * 1]
			cmp al , 0x80
		jae A
		mov byte ptr[ebx][ecx * 1], 0x00
		jmp End

	A:
		mov byte ptr[ebx][ecx * 1], 0xFF

	End:
	   loop StartLoop
	

// YOUR CODE ENDS HERE
		
		pop esi;
		pop edi;
		pop ebx;
	}
	
}


// Part2) This function rotates a given square sized color image 90 degrees clockwise.
// Width and height of the image are equal to "dim".
// Four variables are defined to simplify your implementation.
// Note that you cannot define additional variables.
void imageRotation(unsigned int* image, int dim) {
	
	unsigned int i0, i90, i180, i270;


	__asm {
		push ebx;
		push edi;
		push esi;

// YOUR CODE STARTS HERE

		mov esi, 0 // int i = 0
		mov ecx, 0 // int j = 0

	OuterLoop:
		mov eax, dim
		shr eax, 1 // dim/2 is how many rows to go down
		cmp esi, eax
		jae EndOuter

	InnerLoop:

		mov ebx, image
		mov edx, dim
		dec edx // dim-1 // num of columns
		mov edi, 0 // used to calculate offset

		mov eax, edx
		sub eax, esi
		cmp esi, eax
		jae EndInner

		// 0 
		mov eax, edx
		mov edi, esi
		mul edi
		add eax, ecx
		mov eax, dword ptr[ebx+eax*4]
		mov i0, eax
	
		// 90
		mov eax, edx
		mov edi, ecx
		mul edi
		add eax, edx
		sub eax, esi
		mov eax, dword ptr[ebx+eax*4]
		mov i90, eax

		// 180
		mov eax, edx
		mov edi, edx
		sub edi, esi
		mul edi
		add eax, edx
		sub eax, ecx
		mov eax, dword ptr[ebx+eax*4]
		mov i180, eax

		// 270
		mov eax, edx
		mov edi, edx
		sub edi, ecx
		mul edi
		add eax, esi
		mov eax, dword ptr[ebx+eax*4]
		mov i270, eax

		mov eax, i0
		mov eax, [eax]
		mov edi, i90
		mov edi, [edi]
		mov[eax], edi

		

		inc ecx
		jmp InnerLoop

	EndInner:
		inc esi
		jmp OuterLoop

	EndOuter:





		// Iterate over the red triangle (row by row starting from top left)


		     // compute index of pixel p0, find the corresponding memory address and store it in i0


		     // compute index of pixel p90, find the corresponding memory address and store it in i90


		     // compute index of pixel p180, find the corresponding memory address and store it in i180


		     // compute index of pixel p270, find the corresponding memory address and store it in i270


		     // rotate pixel values: p0 -> p90 -> p180 -> p270 -> p0


// YOUR CODE ENDS HERE
	    
		pop esi;
	    pop edi;
	    pop ebx;
	}
	
}

