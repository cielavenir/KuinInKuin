int64_t a=\{a_.id},b=\{b_.id};
if (a == 0)
{
	return b;
}
if (b == 0)
	return a;
if (a < 0)
	a = -a;
if (b < 0)
	b = -b;
S64 r;
if (b > a)
{
	r = a;
	a = b;
	b = r;
}
for (; ; )
{
	r = a % b;
	if (r == 0)
		return b;
	a = b;
	b = r;
}
