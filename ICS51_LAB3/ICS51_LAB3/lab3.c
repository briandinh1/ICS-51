/*
	ICS 51. Lab #3

	IMPORTATNT NOTES:

	- Noramally, the compiler adds the prologue/epilogue code for the callee. However, when the 
	__declspec(naked) attribute is used, it is the programmer's responsibility to write the 
	prologue/epilogue code. In this lab you need to write prologue/epilogue code sections only 
	for the functions that have been declared with the naked attribute, namely:

	   + findMinIndex
	   + recFactorial
	   + performOperation
	   + fibonacci

	- You cannot define any local variable in the C code. If you need additional variables in your
	assembly code, you have to allocate space for them in the prologue of the function. 

	- You are not allowed to change anything in this file except for adding your assembly code 
    between the lines marked "BEGIN YOUR CODE HERE" and "END YOUR CODE HERE".

	- Remember to fill in your name, student ID below.

	- ONLY submit the lab3.c file for grading.

*/

char *yourName = "Brian Dinh";
char *yourStudentID = "34546266";

/***********************************************************************************************

	PART 1: Selection Sort

	Selection sort is an in-place comparison sort algorithm that works by dividing the input list 
	into two parts: the sublist of items already sorted, which is built up from left to right of 
	the list, and the sublist of items remaining to be sorted that occupy the rest of the list. 
	Initially, the sorted sublist is empty and the unsorted sublist is the entire input list. 
	The algorithm proceeds by finding the smallest element in the unsorted sublist, exchanging it 
	with the leftmost unsorted element (putting it in sorted order), and moving the sublist 
	boundaries one element to the right. To learn more, read: 
	https://en.wikipedia.org/wiki/Selection_sort#Example

	Our implementation here uses a function called "findMinIndex" to find the index of smallest
	element between ith index and jth index of the array. The function "selectionSort" uses
	this function to find the smallest number and exchanges it with the leftmost unsorted element
	in each iteration. You need to implement the behavior of both functions in x86 assembly.

************************************************************************************************/

__declspec(naked) 
int findMinIndex (int integer_array[], int i, int j)
{

// C code to be converted to x86 assembly
/*
    int iMin = i;
    // test against elements after i and before j to find the smallest 
    for ( i ; i < j; i++) {
        // if this element is less, then it is the new minimum   
        if (integer_array[i] < integer_array[iMin]) {
            // found new minimum; remember its index 
            iMin = i;
        }
    }

    return iMin;
*/

	__asm{
			// BEGIN YOUR CODE HERE
		push ebp
		mov ebp, esp
		sub esp, 4
	
		mov esi, i
		mov[ebp-4], esi // int iMin = i

	Begin :
		cmp esi, j
		jge End

		mov edx, [ebp-4]
		mov ebx, integer_array
		mov eax, dword ptr[ebx+esi*4]
		cmp eax, dword ptr[ebx+edx*4]
		jge A

		mov[ebp-4], esi

	A:
		add esi, 1
		jmp Begin

	End:
		mov eax, [ebp-4]
		mov esp, ebp
		pop ebp

		

			// END YOUR CODE HERE
			ret
	}

}

void selectionSort (int integer_array[], int array_size)
{

// C code to be converted to x86 assembly
/*  
	int j;
	int iMin;
	int temp;

	// advance the position through the entire array //
	// (could do j < n-1 because single element is also min element) //
	for (j = 0; j < array_size-1; j++) {

		// find the index of min element in the unsorted a[j .. n-1] //
 		iMin = findMinIndex (integer_array, j, array_size);
 
		if(iMin != j) { // swap values
			temp = integer_array[iMin];
			integer_array[iMin] = integer_array [j];
			integer_array[j] = temp;	
		}
	}
*/

	__asm{
			// BEGIN YOUR CODE HERE

		mov ecx, 0 // j

	Begin:
		mov eax, array_size
		sub eax, 1
		cmp ecx, eax
		jge End
		
		mov edx, integer_array
		push edx
		push ecx
		
		mov esi, array_size
		push esi
		push ecx
		push edx
		
		call findMinIndex
		mov edi, eax
		add esp, 12
		pop ecx
		pop edx

		cmp edi, ecx
		je A

		mov eax, integer_array
		mov ebx, dword ptr[eax + edi*4]
		mov edx, dword ptr[eax + ecx*4]
		mov dword ptr[eax+ecx*4], ebx
		mov dword ptr[eax+edi*4], edx
		
	A:
		add ecx, 1
		jmp Begin

	End:


			// END YOUR CODE HERE
	}

}

/***********************************************************************************************

    PART 2: Recursive Factorial

	Implement two functions that calculates the factorial of an integer not greater than 12.

	The 1st function, factorial (unsigned int input, unsigned int *output), is the non 
	recursive wrapper function which takes the input, sets up the stack, calls the recursive 
	function and finally puts the result in the location pointed to by the pointer argument called
    output.

	The 2nd function, recFactorial(unsigned int n), is the recursive function that actually 
	calculates the factorial.

	Given an unsigned integer number n,we define FACTORIAL(n) as:

        FACTORIAL(n) =  1                   ,    if n = 0 or n = 1
                        n * FACTORIAL(n-1)  ,    if n > 1
		
	Tip:
		
	- You need to use IMUL instruction to multiply unsigned integers. Learn how to use it:
	http://www.cs.virginia.edu/~evans/cs216/guides/x86.html#instructions

************************************************************************************************/

__declspec(naked) 
unsigned int recFactorial (unsigned int n)
{

// C code to be converted to x86 assembly
/*
	if (n<=1) 
		return 1;
	else 
		return n * recFactorial(n-1); 
*/

	__asm{
			// BEGIN YOUR CODE HERE
		
		push ebp
		mov ebp, esp
		push ebx
		push esi
		push edi

		mov eax, n
		cmp eax, 1
		ja A

		mov eax, 1
		jmp B

	A:
		mov eax, n
		sub eax, 1
		push eax
		call recFactorial
		add esp, 4
		mov ebx, n
		imul eax, ebx

	B:
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
		ret

		
			// END YOUR CODE HERE
	}

}

void factorial (unsigned int input, unsigned int *output)
{

// C code to be converted to x86 assembly
/*
	*output = recFactorial (input);
*/

	__asm{
			// BEGIN YOUR CODE HERE
			
		mov eax, input
		push eax
		call recFactorial
		add esp, 4
		mov ecx, output
		mov dword ptr[ecx], eax
		

			// END YOUR CODE HERE
	}

}

/***********************************************************************************************

    PART 3: Vector Operation

	The vectorOperations function gets as parameters 4 arrays of the same length and
	the number of elements in those arrays. 

	For each pair of elements in first_array and second_array, it calls performOperation
	and stores the returned result in the same index of output_array.

	output_array[index] = performOperation(operation_array[index], first_array[index], second_array[index]).

	You also need to implement the body of performOperation function according to the provided
	C code.
  
************************************************************************************************/

__declspec(naked) 
int performOperation (int operation, int first, int second)
{

// C code to be converted to x86 assembly
/*
	if (operation==0) 
		return first + second;
	else if (operation==1)
		return first - second;
	else if (operation==2)
		return (first > second)? first : second;
	else if (operation==3)
		return (first < second)? first : second;
	else
		return first;
*/

	__asm {
			// BEGIN YOUR CODE HERE

		push ebp
		mov ebp, esp
		push ebx
		push esi
		push edi

	// Op 0
		mov eax, operation
		cmp eax, 0
		jne Op1

		mov eax, first
		add eax, second
		jmp End

	Op1:
		mov eax, operation
		cmp eax, 1
		jne Op2

		mov eax, first
		sub eax, second
		jmp End

	Op2:
		mov eax, operation
		cmp eax, 2
		jne Op3

		mov eax, first
		cmp eax, second
		jle A

		mov eax, first
		jmp End

	A:
		mov eax, second
		jmp End


	Op3:
		mov eax, operation
		cmp eax, 3
		jne retFirst

		mov eax, first
		cmp eax, second
		jge B

		mov eax, first
		jmp End

	B:
		mov eax, second
		jmp End

	retFirst:
		mov eax, first

	End:
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp
			// END YOUR CODE HERE
			ret
	}
  
}

void vectorOperations (int number_of_elements, int *first_array, int *second_array, 
					   int *operation_array, int *output_array)
{
// C code to be converted to x86 assembly
/*
	int i;
	for (i=0; i<number_of_elements; i++)
	{
		output_array[i] = performOperation(operation_array[i], first_array[i], second_array[i]);
	}
*/

	__asm {
			// BEGIN YOUR CODE HERE

		mov ebx, 0 // i

	
	Begin:
		cmp ebx, number_of_elements
		jge End

		mov ecx, second_array
		mov edx, dword ptr[ecx+ebx*4]
		push edx

		mov ecx, first_array
		mov edx, dword ptr[ecx+ebx*4]
		push edx

		mov ecx, operation_array
		mov edx, dword ptr[ecx+ebx*4]
		push edx

		call performOperation
		add esp, 12
		mov ecx, ebx
		mov edx, output_array
		mov dword ptr[edx+ecx*4], eax

		add ebx, 1
		jmp Begin

	End:

			// END YOUR CODE HERE
	}
}

/***********************************************************************************************

    PART 4: Change Letter Case

	You are given an array of characters (a string) that may contain numbers, upper and	lower case 
    letters, punctuations symbols, etc. Implement a function to convert all uppercase letters to 
    lowercase and vice versa. 
    For example "ThisIsTheFinalLabOf51" should be converted to "tHISiStHEfINALlABoF51". 
	The function has the following argument: An array of charaters and the size of the array. The 
	array should be changed in place. There is no explicit output for this function.

	Tips:

	- Note that each element of the arrays is a char (1 byte). You can NOT use a 32-bit register 
	to read/write from/to a memory location. Instead you should use the 8-bit version of the x86
	general-purpose registers. 
	More info: http://www.cs.virginia.edu/~evans/cs216/guides/x86.html#registers

	- The link to the ascii codes is here: http://www.asciitable.com/

************************************************************************************************/

void changeCase (char char_array[], int array_size ) {

	__asm{
			// BEGIN YOUR CODE HERE
		mov edx, char_array
		mov ebx, array_size
		mov ecx, 0

	Begin:
		cmp ecx, ebx
		jge End

		mov al, byte ptr[edx+ecx*1]
		cmp al, 64
		jle B

		cmp al, 91
		jge A

		add byte ptr [edx+ecx*1], 32
		jmp B

	A:
		cmp al, 123
		jge B

		sub byte ptr[edx+ecx*1], 32

	B:
		inc ecx
		jmp Begin

	End:

			// END YOUR CODE HERE
	}
}

/***********************************************************************************************

    PART 5: Fibonacci

	Implement a recursive function in assembly which returns the Nth number in the fibbonaci
	series (1, 1, 2, 3, 5, 8, ...).

************************************************************************************************/

__declspec(naked) 
unsigned int fibonacci (unsigned int n) {

// C code to be converted to x86 assembly
/*
	if (n < 2) 
		return 1;
	else 
		return fibonacci(n - 1) + fibonacci(n - 2);
*/

	__asm{
			// BEGIN YOUR CODE HERE

		push ebp
		mov ebp, esp
		push ebx
		push esi
		push edi

		mov eax, n
		cmp eax, 2
		jae A

		mov eax, 1
		jmp B

	A:
		mov eax, n
		sub eax, 1
		push eax
		call fibonacci
		add esp, 4

		mov esi, eax
		mov ebx, n
		sub ebx, 2
		push ebx
		call fibonacci
		add esp, 4
		add eax, esi

	B:
		pop edi
		pop esi
		pop ebx
		mov esp, ebp
		pop ebp


			// END YOUR CODE HERE
			ret
	}
}