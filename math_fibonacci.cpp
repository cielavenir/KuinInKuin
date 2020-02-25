if (\{n_.id} < 0)
{
	THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
	return 0;
}
if (\{n_.id} > 92)
{
	THROW(EXCPT_DBG_INT_OVERFLOW);
	return 0;
}
return FibonacciNumbers[\{n_.id}];
