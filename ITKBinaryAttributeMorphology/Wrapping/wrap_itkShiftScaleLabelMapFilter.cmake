WRAP_CLASS("itk::ShiftScaleLabelMapFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_LM${d}}" "${ITKT_LM${d}}")
  ENDFOREACH(d)
END_WRAP_CLASS()
