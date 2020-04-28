SELECT op.nom, ma.designationM 
FROM operateur op, machine ma, qualif_sur qual
WHERE op.numero       = qual.numero	AND
      qual.referenceM = ma.referenceM 	AND
      ma.designationM = "Perceuse"
;
