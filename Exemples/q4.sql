SELECT op.nom, m.designationM
FROM operateur op, qualif_sur qs, machine m, piece p
WHERE op.numero     = qs.numero		AND
      qs.referenceM = m.referenceM 	AND
      p.referenceM  = m.referenceM	AND
      p.numero      = op.numero
;
