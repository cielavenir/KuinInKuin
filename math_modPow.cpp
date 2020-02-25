if(\{value_.id}<0)THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
if(\{exponent_.id}<0)THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
if(\{modulus_.id}<0)THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
return (int64_t)ModPow((uint64_t)\{value_.id}, (uint64_t)\{exponent_.id}, (uint64_t)\{modulus_.id});
