IntList* top = NULL;
IntList* bottom = NULL;
int num = 0;
while (n > 1)
{
	S64 factor = FindFactor(n, 1);
	IntList* node = (IntList*)malloc(sizeof(IntList));
	node->Value = factor;
	node->Next = NULL;
	if (top == NULL)
	{
		top = node;
		bottom = node;
	}
	else
	{
		bottom->Next = node;
		bottom = node;
	}
	num++;
	n /= factor;
}
Array<int64_t>
U8* result = (U8*)malloc(0x10 + sizeof(S64) * (size_t)num);
((S64*)result)[0] = DefaultRefCntFunc;
((S64*)result)[1] = (S64)num;
{
	IntList* ptr = top;
	S64* dst = (S64*)result + 2;
	while (ptr != NULL)
	{
		IntList* ptr2 = ptr;
		ptr = ptr->Next;
		*dst = ptr2->Value;
		dst++;
		FreeMem(ptr2);
	}
}
qsort((S64*)result + 2, (size_t)num, sizeof(S64), CmpInt);
return result;
