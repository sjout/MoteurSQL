SELECT nom, referenceM
FROM operateur op, qualif_sur qual 
WHERE op.numero  = qual.numero AND
      referenceM = "M12"
;
