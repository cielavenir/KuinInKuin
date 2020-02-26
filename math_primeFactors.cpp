IntList* top = NULL;
IntList* bottom = NULL;
int num = 0;
while (\{n_.id} > 1)
{
	S64 factor = FindFactor(\{n_.id});
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
	\{n_.id} /= factor;
}
Array_<int64_t> *result=new Array_<int64_t>();
result->L = num;
result->B = new int64_t[num+1];
{
	IntList* ptr = top;
	S64* dst = result->B;
	while (ptr != NULL)
	{
		IntList* ptr2 = ptr;
		ptr = ptr->Next;
		*dst = ptr2->Value;
		dst++;
		free(ptr2);
	}
}
qsort(result->B, (size_t)num, sizeof(S64), CmpInt);
return result;
