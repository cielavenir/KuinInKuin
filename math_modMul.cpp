if(\{a_.id}<0)THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
if(\{b_.id}<0)THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
if(\{modulus_.id}<0)THROW(EXCPT_DBG_ARG_OUT_DOMAIN);
return (int64_t)ModMul((uint64_t)\{a_.id}, (uint64_t)\{b_.id}, (uint64_t)\{modulus_.id});
