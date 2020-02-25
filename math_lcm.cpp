if (a == 0)
{
	if(b==0)THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
	return 0;
}
if (b == 0)
	return 0;
if (a < 0)
	a = -a;
if (b < 0)
	b = -b;
return a / _gcd(a, b) * b;
