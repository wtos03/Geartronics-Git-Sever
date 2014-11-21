/************************************************************************/
/*      H8SX/1653 Group Include File                       Ver 2.0      */
/************************************************************************/
struct st_usb {                                         /* struct USBF  */
              union {                                   /* IFR0         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char BRST    :1;    /*    BRST      */
                           unsigned char EP1FULL :1;    /*    EP1FULL   */
                           unsigned char EP2TR   :1;    /*    EP2TR     */
                           unsigned char EP2EMPTY:1;    /*    EP2EMPTY  */
                           unsigned char SETUPTS :1;    /*    SETUPTS   */
                           unsigned char EP0oTS  :1;    /*    EP0oTS    */
                           unsigned char EP0iTR  :1;    /*    EP0iTR    */
                           unsigned char EP0iTS  :1;    /*    EP0iTS    */
                           }      BIT;                  /*              */
                    }           IFR0;                   /*              */
              union {                                   /* IFR1         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char       :4;      /*              */
                           unsigned char VBUSMN:1;      /*    VBUSMN    */
                           unsigned char EP3TR :1;      /*    EP3TR     */
                           unsigned char EP3TS :1;      /*    EP3TS     */
                           unsigned char VBUSF :1;      /*    VBUSF     */
                           }      BIT;                  /*              */
                    }           IFR1;                   /*              */
              union {                                   /* IFR2         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :2;       /*              */
                           unsigned char SURSS:1;       /*    SURSS     */
                           unsigned char SURSF:1;       /*    SURSF     */
                           unsigned char CFDN :1;       /*    CFDN      */
                           unsigned char      :1;       /*              */
                           unsigned char SETC :1;       /*    SETC      */
                           unsigned char SETI :1;       /*    SETI      */
                           }      BIT;                  /*              */
                    }           IFR2;                   /*              */
              char              wk1;                    /*              */
              union {                                   /* IER0         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char BRST    :1;    /*    BRST      */
                           unsigned char EP1FULL :1;    /*    EP1FULL   */
                           unsigned char EP2TR   :1;    /*    EP2TR     */
                           unsigned char EP2EMPTY:1;    /*    EP2EMPTY  */
                           unsigned char SETUPTS :1;    /*    SETUPTS   */
                           unsigned char EP0oTS  :1;    /*    EP0oTS    */
                           unsigned char EP0iTR  :1;    /*    EP0iTR    */
                           unsigned char EP0iTS  :1;    /*    EP0iTS    */
                           }      BIT;                  /*              */
                    }           IER0;                   /*              */
              union {                                   /* IER1         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char       :5;      /*              */
                           unsigned char EP3TR :1;      /*    EP3TR     */
                           unsigned char EP3TS :1;      /*    EP3TS     */
                           unsigned char VBUSF :1;      /*    VBUSF     */
                           }      BIT;                  /*              */
                    }           IER1;                   /*              */
              union {                                   /* IER2         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char SSRSME:1;      /*    SSRSME    */
                           unsigned char       :2;      /*              */
                           unsigned char SURSE :1;      /*    SURSE     */
                           unsigned char CFDN  :1;      /*    CFDN      */
                           unsigned char       :1;      /*              */
                           unsigned char SETCE :1;      /*    SETCE     */
                           unsigned char SETIE :1;      /*    SETIE     */
                           }      BIT;                  /*              */
                    }           IER2;                   /*              */
              char              wk2;                    /*              */
              union {                                   /* ISR0         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char BRST    :1;    /*    BRST      */
                           unsigned char EP1FULL :1;    /*    EP1FULL   */
                           unsigned char EP2TR   :1;    /*    EP2TR     */
                           unsigned char EP2EMPTY:1;    /*    EP2EMPTY  */
                           unsigned char SETUPTS :1;    /*    SETUPTS   */
                           unsigned char EP0oTS  :1;    /*    EP0oTS    */
                           unsigned char EP0iTR  :1;    /*    EP0iTR    */
                           unsigned char EP0iTS  :1;    /*    EP0iTS    */
                           }      BIT;                  /*              */
                    }           ISR0;                   /*              */
              union {                                   /* ISR1         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char       :5;      /*              */
                           unsigned char EP3TR :1;      /*    EP3TR     */
                           unsigned char EP3TS :1;      /*    EP3TS     */
                           unsigned char VBUSF :1;      /*    VBUSF     */
                           }      BIT;                  /*              */
                    }           ISR1;                   /*              */
              union {                                   /* ISR2         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :3;       /*              */
                           unsigned char SURSE:1;       /*    SURSE     */
                           unsigned char CFDN :1;       /*    CFDN      */
                           unsigned char      :1;       /*              */
                           unsigned char SETCE:1;       /*    SETCE     */
                           unsigned char SETIE:1;       /*    SETIE     */
                           }      BIT;                  /*              */
                    }           ISR2;                   /*              */
              char              wk3;                    /*              */
              unsigned char     EPDR0i;                 /* EPDR0i       */
              unsigned char     EPDR0o;                 /* EPDR0o       */
              unsigned char     EPDR0s;                 /* EPDR0s       */
              char              wk4;                    /*              */
              unsigned char     EPDR1;                  /* EPDR1        */
              char              wk5[3];                 /*              */
              unsigned char     EPDR2;                  /* EPDR2        */
              char              wk6[3];                 /*              */
              unsigned char     EPDR3;                  /* EPDR3        */
              char              wk7[11];                /*              */
              unsigned char     EPSZ0o;                 /* EPSZ0o       */
              unsigned char     EPSZ1;                  /* EPSZ1        */
              char              wk8;                    /*              */
              union {                                   /* DASTS        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char       :2;      /*              */
                           unsigned char EP3DE :1;      /*    EP3DE     */
                           unsigned char EP2DE :1;      /*    EP2DE     */
                           unsigned char       :3;      /*              */
                           unsigned char EP0iDE:1;      /*    EP0iDE    */
                           }      BIT;                  /*              */
                    }           DASTS;                  /*              */
              union {                                   /* FCLR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char        :1;     /*              */
                           unsigned char EP3CLR :1;     /*    EP3CLR    */
                           unsigned char EP1CLR :1;     /*    EP1CLR    */
                           unsigned char EP2CLR :1;     /*    EP2CLR    */
                           unsigned char        :2;     /*              */
                           unsigned char EP0oCLR:1;     /*    EP0oCLR   */
                           unsigned char EP0iCLR:1;     /*    EP0iCLR   */
                           }      BIT;                  /*              */
                    }           FCLR;                   /*              */
              char              wk9;                    /*              */
              union {                                   /* EPSTL        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char       :4;      /*              */
                           unsigned char EP3STL:1;      /*    EP3STL    */
                           unsigned char EP2STL:1;      /*    EP2STL    */
                           unsigned char EP1STL:1;      /*    EP1STL    */
                           unsigned char EP0STL:1;      /*    EP0STL    */
                           }      BIT;                  /*              */
                    }           EPSTL;                  /*              */
              char              wk10;                   /*              */
              union {                                   /* TRG          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char         :1;    /*              */
                           unsigned char EP3PKTE :1;    /*    EP3PKTE   */
                           unsigned char EP1RDFN :1;    /*    EP1RDFN   */
                           unsigned char EP2PKTE :1;    /*    EP2PKTE   */
                           unsigned char         :1;    /*              */
                           unsigned char EP0sRDFN:1;    /*    EP0sRDFN  */
                           unsigned char EP0oRDFN:1;    /*    EP0oRDFN  */
                           unsigned char EP0iPKTE:1;    /*    EP0iPKTE  */
                           }      BIT;                  /*              */
                    }           TRG;                    /*              */
              union {                                   /* DMAR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char         :5;    /*              */
                           unsigned char PULLUP_E:1;    /*    PULLUP_E  */
                           unsigned char EP2DMAE :1;    /*    EP2DMAE   */
                           unsigned char EP1DMAE :1;    /*    EP1DMAE   */
                           }      BIT;                  /*              */
                    }           DMAR;                   /*              */
              union {                                   /* CVR          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CNFV:2;        /*    CNFV      */
                           unsigned char INTV:2;        /*    INTV      */
                           unsigned char     :1;        /*              */
                           unsigned char ALTV:3;        /*    ALTV      */
                           }      BIT;                  /*              */
                    }           CVR;                    /*              */
              union {                                   /* CTLR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :3;       /*              */
                           unsigned char RWUPS:1;       /*    RWUPS     */
                           unsigned char RSME :1;       /*    RSME      */
                           unsigned char PWMD :1;       /*    PWMD      */
                           unsigned char ASCE :1;       /*    ASCE      */
                           }      BIT;                  /*              */
                    }           CTLR;                   /*              */
              char              wk11[2];                /*              */
              unsigned char     EPIR;                   /* EPIR         */
              char              wk12[17];               /*              */
              union {                                   /* TRNTREG0     */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PTSTE  :1;     /*    PTSTE     */
                           unsigned char        :3;     /*              */
                           unsigned char SUSPEND:1;     /*    SUSPEND   */
                           unsigned char txenl  :1;     /*    txenl     */
                           unsigned char Txse0  :1;     /*    Txse0     */
                           unsigned char Txdata :1;     /*    Txdata    */
                           }      BIT;                  /*              */
                    }           TRNTREG0;               /*              */
              union {                                   /* TRNTREG1     */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char          :5;   /*              */
                           unsigned char xver_data:1;   /*    xver_data */
                           unsigned char dpls     :1;   /*    dpls      */
                           unsigned char dmns     :1;   /*    dmns      */
                           }      BIT;                  /*              */
                     }          TRNTREG1;               /*              */
};                                                      /*              */
struct st_p1 {                                          /* struct P1    */
             unsigned char      DDR;                    /* P1DDR        */
             char               wk1[15];                /*              */
             union {                                    /* P1ICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[943];               /*              */
             union {                                    /* P1PORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk3[15];                /*              */
             union {                                    /* P1DR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_p2 {                                          /* struct P2    */
             unsigned char      DDR;                    /* P2DDR        */
             char               wk1[15];                /*              */
             union {                                    /* P2ICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[42];                /*              */
             union {                                    /* P2ODR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ODR;                    /*              */
             char               wk3[900];               /*              */
             union {                                    /* P2PORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk4[15];                /*              */
             union {                                    /* P2DR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_p5 {                                          /* struct P5    */
             union {                                    /* P5ICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk1[943];               /*              */
             union {                                    /* P5PORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
};                                                      /*              */
struct st_p6 {                                          /* struct P6    */
             unsigned char      DDR;                    /* P6DDR        */
             char               wk1[15];                /*              */
             union {                                    /* P6ICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :2;           /*    Bit 7,6   */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[943];               /*              */
             union {                                    /* P6PORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :2;           /*    Bit 7,6   */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk3[15];                /*              */
             union {                                    /* P6DR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :2;           /*    Bit 7,6   */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pa {                                          /* struct PA    */
             unsigned char      DDR;                    /* PADDR        */
             char               wk1[15];                /*              */
             union {                                    /* PAICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[943];               /*              */
             union {                                    /* PAPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk3[15];                /*              */
             union {                                    /* PADR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pb {                                          /* struct PB    */
             unsigned char      DDR;                    /* PBDDR        */
             char               wk1[15];                /*              */
             union {                                    /* PBICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :4;           /*    Bit 7-4   */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[943];               /*              */
             union {                                    /* PBPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :4;           /*    Bit 7-4   */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk3[15];                /*              */
             union {                                    /* PBDR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :4;           /*    Bit 7-4   */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pd {                                          /* struct PD    */
             unsigned char      DDR;                    /* PDDDR        */
             char               wk1[15];                /*              */
             union {                                    /* PDICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[23];                /*              */
             union {                                    /* PDPCR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PCR;                    /*              */
             char               wk3[919];               /*              */
             union {                                    /* PDPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk4[15];                /*              */
             union {                                    /* PDDR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pe {                                          /* struct PE    */
             unsigned char      DDR;                    /* PEDDR        */
             char               wk1[15];                /*              */
             union {                                    /* PEICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[23];                /*              */
             union {                                    /* PEPCR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PCR;                    /*              */
             char               wk3[919];               /*              */
             union {                                    /* PEPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk4[15];                /*              */
             union {                                    /* PEDR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pf {                                          /* struct PF    */
             unsigned char      DDR;                    /* PFDDR        */
             char               wk1[15];                /*              */
             union {                                    /* PFICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk2[23];                /*              */
             union {                                    /* PFPCR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PCR;                    /*              */
             char               wk3[6];                 /*              */
             union {                                    /* PFODR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ODR;                    /*              */
             char               wk4[912];               /*              */
             union {                                    /* PFPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk5[15];                /*              */
             union {                                    /* PFDR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_ph {                                          /* struct PH    */
             union {                                    /* PHPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk1[3];                 /*              */
             union {                                    /* PHDR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
             char               wk2[3];                 /*              */
             unsigned char      DDR;                    /* PHDDR        */
             char               wk3[3];                 /*              */
             union {                                    /* PHICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk4[11];                /*              */
             union {                                    /* PHPCR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PCR;                    /*              */
};                                                      /*              */
struct st_pi {                                          /* struct PI    */
             union {                                    /* PIPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             char               wk1[3];                 /*              */
             union {                                    /* PIDR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
             char               wk2[3];                 /*              */
             unsigned char      DDR;                    /* PIDDR        */
             char               wk3[3];                 /*              */
             union {                                    /* PIICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
             char               wk4[11];                /*              */
             union {                                    /* PIPCR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PCR;                    /*              */
};                                                      /*              */
struct st_pm {                                          /* struct PM    */
             unsigned char      DDR;                    /* PMDDR        */
             union {                                    /* PMDR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
             union {                                    /* PMPORT       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            PORT;                   /*              */
             union {                                    /* PMICR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :3;           /*    Bit 7-5   */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            ICR;                    /*              */
};                                                      /*              */
struct st_pfc {                                         /* struct PFC   */
              union {                                   /* PFCR0        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CS7E:1;        /*    CS7E      */
                           unsigned char CS6E:1;        /*    CS6E      */
                           unsigned char CS5E:1;        /*    CS5E      */
                           unsigned char CS4E:1;        /*    CS4E      */
                           unsigned char CS3E:1;        /*    CS3E      */
                           unsigned char CS2E:1;        /*    CS2E      */
                           unsigned char CS1E:1;        /*    CS1E      */
                           unsigned char CS0E:1;        /*    CS0E      */
                           }      BIT;                  /*              */
                    }           PFCR0;                  /*              */
              union {                                   /* PFCR1        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CS7S:2;        /*    CS7S      */
                           unsigned char CS6S:2;        /*    CS6S      */
                           unsigned char CS5S:2;        /*    CS5S      */
                           }      BIT;                  /*              */
                    }           PFCR1;                  /*              */
              union {                                   /* PFCR2        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :1;       /*              */
                           unsigned char CS2S :1;       /*    CS2S      */
                           unsigned char BSS  :1;       /*    BSS       */
                           unsigned char BSE  :1;       /*    BSE       */
                           unsigned char      :1;       /*              */
                           unsigned char RDWRE:1;       /*    RDWRE     */
                           unsigned char ASOE :1;       /*    ASOE      */
                           }      BIT;                  /*              */
                    }           PFCR2;                  /*              */
              char              wk1;                    /*              */
              union {                                   /* PFCR4        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :3;        /*              */
                           unsigned char A20E:1;        /*    A20E      */
                           unsigned char A19E:1;        /*    A19E      */
                           unsigned char A18E:1;        /*    A18E      */
                           unsigned char A17E:1;        /*    A17E      */
                           unsigned char A16E:1;        /*    A16E      */
                           }      BIT;                  /*              */
                    }           PFCR4;                  /*              */
              char              wk2;                    /*              */
              union {                                   /* PFCR6        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char       :1;      /*              */
                           unsigned char LHWROE:1;      /*    LHWROE    */
                           unsigned char       :2;      /*              */
                           unsigned char TCLKS :1;      /*    TCLKS     */
                           }      BIT;                  /*              */
                    }           PFCR6;                  /*              */
              union {                                   /* PFCR7        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char DMAS3:2;       /*    DMAS3     */
                           unsigned char DMAS2:2;       /*    DMAS2     */
                           unsigned char DMAS1:2;       /*    DMAS1     */
                           unsigned char DMAS0:2;       /*    DMAS0     */
                           }      BIT;                  /*              */
                    }           PFCR7;                  /*              */
              char              wk3;                    /*              */
              union {                                   /* PFCR9        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TPUMS5 :1;     /*    TPUMS5    */
                           unsigned char TPUMS4 :1;     /*    TPUMS4    */
                           unsigned char TPUMS3A:1;     /*    TPUMS3A   */
                           unsigned char TPUMS3B:1;     /*    TPUMS3B   */
                           }      BIT;                  /*              */
                    }           PFCR9;                  /*              */
              char              wk4;                    /*              */
              union {                                   /* PFCRB        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :4;       /*              */
                           unsigned char ITS11:1;       /*    ITS11     */
                           unsigned char ITS10:1;       /*    ITS10     */
                           unsigned char ITS9 :1;       /*    ITS9      */
                           unsigned char ITS8 :1;       /*    ITS8      */
                           }      BIT;                  /*              */
                    }           PFCRB;                  /*              */
              union {                                   /* PFCRC        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ITS7:1;        /*    ITS7      */
                           unsigned char ITS6:1;        /*    ITS6      */
                           unsigned char ITS5:1;        /*    ITS5      */
                           unsigned char ITS4:1;        /*    ITS4      */
                           unsigned char ITS3:1;        /*    ITS3      */
                           unsigned char ITS2:1;        /*    ITS2      */
                           unsigned char ITS1:1;        /*    ITS1      */
                           unsigned char ITS0:1;        /*    ITS0      */
                           }      BIT;                  /*              */
                    }           PFCRC;                  /*              */
};                                                      /*              */
struct st_intc {                                        /* struct INTC  */
               union {                                  /* SSIER        */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char SSI11:1;      /*    SSI11     */
                            unsigned char SSI10:1;      /*    SSI10     */
                            unsigned char SSI9 :1;      /*    SSI9      */
                            unsigned char SSI8 :1;      /*    SSI8      */
                            unsigned char SSI7 :1;      /*    SSI7      */
                            unsigned char SSI6 :1;      /*    SSI6      */
                            unsigned char SSI5 :1;      /*    SSI5      */
                            unsigned char SSI4 :1;      /*    SSI4      */
                            unsigned char SSI3 :1;      /*    SSI3      */
                            unsigned char SSI2 :1;      /*    SSI2      */
                            unsigned char SSI1 :1;      /*    SSI1      */
                            unsigned char SSI0 :1;      /*    SSI0      */
                            }     BIT;                  /*              */
                     }          SSIER;                  /*              */
               char             wk1[368];               /*              */
               union {                                  /* IPRA         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ0:3;      /*    IRQ0      */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ1:3;      /*    IRQ1      */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ2:3;      /*    IRQ2      */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ3:3;      /*    IRQ3      */
                            }     BIT;                  /*              */
                     }          IPRA;                   /*              */
               union {                                  /* IPRB         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ4:3;      /*    IRQ4      */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ5:3;      /*    IRQ5      */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ6:3;      /*    IRQ6      */
                            unsigned char      :1;      /*              */
                            unsigned char _IRQ7:3;      /*    IRQ7      */
                            }     BIT;                  /*              */
                     }          IPRB;                   /*              */
               union {                                  /* IPRC         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char       :1;     /*              */
                            unsigned char _IRQ8 :3;     /*    IRQ8      */
                            unsigned char       :1;     /*              */
                            unsigned char _IRQ9 :3;     /*    IRQ9      */
                            unsigned char       :1;     /*              */
                            unsigned char _IRQ10:3;     /*    IRQ10     */
                            unsigned char       :1;     /*              */
                            unsigned char _IRQ11:3;     /*    IRQ11     */
                            }     BIT;                  /*              */
                     }          IPRC;                   /*              */
               char             wk2[2];                 /*              */
               union {                                  /* IPRE         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :5;       /*              */
                            unsigned char _WDT:3;       /*    WDT       */
                            }     BIT;                  /*              */
                     }          IPRE;                   /*              */
               union {                                  /* IPRF         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :5;      /*              */
                            unsigned char _AD  :3;      /*    A/D       */
                            unsigned char      :1;      /*              */
                            unsigned char _TPU0:3;      /*    TPU0      */
                            unsigned char      :1;      /*              */
                            unsigned char _TPU1:3;      /*    TPU1      */
                            }     BIT;                  /*              */
                     }          IPRF;                   /*              */
               union {                                  /* IPRG         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :1;      /*              */
                            unsigned char _TPU2:3;      /*    TPU2      */
                            unsigned char      :1;      /*              */
                            unsigned char _TPU3:3;      /*    TPU3      */
                            unsigned char      :1;      /*              */
                            unsigned char _TPU4:3;      /*    TPU4      */
                            unsigned char      :1;      /*              */
                            unsigned char _TPU5:3;      /*    TPU5      */
                            }     BIT;                  /*              */
                     }          IPRG;                   /*              */
               union {                                  /* IPRH         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :1;      /*              */
                            unsigned char _TMR0:3;      /*    TMR0      */
                            unsigned char      :1;      /*              */
                            unsigned char _TMR1:3;      /*    TMR1      */
                            unsigned char      :1;      /*              */
                            unsigned char _TMR2:3;      /*    TMR2      */
                            unsigned char      :1;      /*              */
                            unsigned char _TMR3:3;      /*    TMR3      */
                            }     BIT;                  /*              */
                     }          IPRH;                   /*              */
               union {                                  /* IPRI         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char       :1;     /*              */
                            unsigned char _DMAC0:3;     /*    DMAC0     */
                            unsigned char       :1;     /*              */
                            unsigned char _DMAC1:3;     /*    DMAC1     */
                            unsigned char       :1;     /*              */
                            unsigned char _DMAC2:3;     /*    DMAC2     */
                            unsigned char       :1;     /*              */
                            unsigned char _DMAC3:3;     /*    DMAC3     */
                            }     BIT;                  /*              */
                     }          IPRI;                   /*              */
               char             wk3[2];                 /*              */
               union {                                  /* IPRK         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :1;      /*              */
                            unsigned char _DMAC:3;      /*    DMAC      */
                            unsigned char      :4;      /*              */
                            unsigned char      :1;      /*              */
                            unsigned char _SCI0:3;      /*    SCI0      */
                            unsigned char      :1;      /*              */
                            unsigned char _SCI1:3;      /*    SCI1      */
                            }     BIT;                  /*              */
                     }          IPRK;                   /*              */
               union {                                  /* IPRL         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :1;      /*              */
                            unsigned char _SCI2:3;      /*    SCI2      */
                            unsigned char      :4;      /*              */
                            unsigned char      :1;      /*              */
                            unsigned char _SCI4:3;      /*    SCI4      */
                            }     BIT;                  /*              */
                     }          IPRL;                   /*              */
               char             wk4[8];                 /*              */
               union {                                  /* IPRQ         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :8;      /*              */
                            unsigned char      :1;      /*              */
                            unsigned char _IIC2:3;      /*    IIC2      */
                            unsigned char      :1;      /*              */
                            unsigned char _SCI5:3;      /*    SCI5      */
                            }     BIT;                  /*              */
                     }          IPRQ;                   /*              */
               union {                                  /* IPRR         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char        :1;    /*              */
                            unsigned char _SCI6  :3;    /*    SCI6      */
                            unsigned char        :1;    /*              */
                            unsigned char _TMR4_7:3;    /*    TMR4_7    */
                            unsigned char        :1;    /*              */
                            unsigned char _USB   :3;    /*    USB       */
                            unsigned char        :1;    /*              */
                            unsigned char _USBres:3;    /*    USBresume */
                            }     BIT;                  /*              */
                     }          IPRR;                   /*              */
               char             wk5[4];                 /*              */
               union {                                  /* ISCR         */
                     unsigned long LONG;                /*  Long Access */
                     struct {                           /*  Word Access */
                            unsigned int H;             /*    ISCRH     */
                            unsigned int L;             /*    ISCRL     */
                            }      WORD;                /*              */
                     struct {                           /*  Bit  Access */
                            unsigned char        :8;    /*              */
                            unsigned char IRQ11SC:2;    /*    IRQ11SC   */
                            unsigned char IRQ10SC:2;    /*    IRQ10SC   */
                            unsigned char IRQ9SC :2;    /*    IRQ9SC    */
                            unsigned char IRQ8SC :2;    /*    IRQ8SC    */
                            unsigned char IRQ7SC :2;    /*    IRQ7SC    */
                            unsigned char IRQ6SC :2;    /*    IRQ6SC    */
                            unsigned char IRQ5SC :2;    /*    IRQ5SC    */
                            unsigned char IRQ4SC :2;    /*    IRQ4SC    */
                            unsigned char IRQ3SC :2;    /*    IRQ3SC    */
                            unsigned char IRQ2SC :2;    /*    IRQ2SC    */
                            unsigned char IRQ1SC :2;    /*    IRQ1SC    */
                            unsigned char IRQ0SC :2;    /*    IRQ0SC    */
                            }      BIT;                 /*              */
                     }          ISCR;                   /*              */
               char             wk6[454];               /*              */
               union {                                  /* INTCR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :2;      /*              */
                            unsigned char INTM :2;      /*    INTM      */
                            unsigned char NMIEG:1;      /*    NMIEG     */
                            }      BIT;                 /*              */
                     }          INTCR;                  /*              */
               union {                                  /* CPUPCR       */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CPUPCE:1;     /*    CPUPCE    */
                            unsigned char DTCP  :3;     /*    DTCP      */
                            unsigned char IPSETE:1;     /*    IPSETE    */
                            unsigned char CPUP  :3;     /*    CPUP      */
                            }      BIT;                 /*              */
                     }          CPUPCR;                 /*              */
               union {                                  /* IER          */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char       :4;     /*              */
                            unsigned char IRQ11E:1;     /*    IRQ11E    */
                            unsigned char IRQ10E:1;     /*    IRQ10E    */
                            unsigned char IRQ9E :1;     /*    IRQ9E     */
                            unsigned char IRQ8E :1;     /*    IRQ8E     */
                            unsigned char IRQ7E :1;     /*    IRQ7E     */
                            unsigned char IRQ6E :1;     /*    IRQ6E     */
                            unsigned char IRQ5E :1;     /*    IRQ5E     */
                            unsigned char IRQ4E :1;     /*    IRQ4E     */
                            unsigned char IRQ3E :1;     /*    IRQ3E     */
                            unsigned char IRQ2E :1;     /*    IRQ2E     */
                            unsigned char IRQ1E :1;     /*    IRQ1E     */
                            unsigned char IRQ0E :1;     /*    IRQ0E     */
                            }     BIT;                  /*              */
                     }          IER;                    /*              */
               union {                                  /* ISR          */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char       :4;     /*              */
                            unsigned char IRQ11F:1;     /*    IRQ11F    */
                            unsigned char IRQ10F:1;     /*    IRQ10F    */
                            unsigned char IRQ9F :1;     /*    IRQ9F     */
                            unsigned char IRQ8F :1;     /*    IRQ8F     */
                            unsigned char IRQ7F :1;     /*    IRQ7F     */
                            unsigned char IRQ6F :1;     /*    IRQ6F     */
                            unsigned char IRQ5F :1;     /*    IRQ5F     */
                            unsigned char IRQ4F :1;     /*    IRQ4F     */
                            unsigned char IRQ3F :1;     /*    IRQ3F     */
                            unsigned char IRQ2F :1;     /*    IRQ2F     */
                            unsigned char IRQ1F :1;     /*    IRQ1F     */
                            unsigned char IRQ0F :1;     /*    IRQ0F     */
                            }     BIT;                  /*              */
                     }          ISR;                    /*              */
};                                                      /*              */
struct st_dmac0 {                                       /* struct DMAC0 */
                void           *DSAR;                   /* DSAR         */
                void           *DDAR;                   /* DDAR         */
                unsigned long   DOFR;                   /* DOFR         */
                unsigned long   DTCR;                   /* DTCR         */
                unsigned long   DBSR;                   /* DBSR         */
                union {                                 /* DMDR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char DTE  :1;     /*    DTE       */
                             unsigned char DACKE:1;     /*    DACKE     */
                             unsigned char TENDE:1;     /*    TENDE     */
                             unsigned char      :1;     /*              */
                             unsigned char DREQS:1;     /*    DREQS     */
                             unsigned char NRD  :1;     /*    NRD       */
                             unsigned char      :2;     /*              */
                             unsigned char ACT  :1;     /*    ACT       */
                             unsigned char      :3;     /*              */
                             unsigned char ERRF :1;     /*    ERRF      */
                             unsigned char      :1;     /*              */
                             unsigned char ESIF :1;     /*    ESIF      */
                             unsigned char DTIF :1;     /*    DTIF      */
                             unsigned char DTSZ :2;     /*    DTSZ      */
                             unsigned char MDS  :2;     /*    MDS       */
                             unsigned char TSEIE:1;     /*    TSEIE     */
                             unsigned char      :1;     /*              */
                             unsigned char ESIE :1;     /*    ESIE      */
                             unsigned char DTIE :1;     /*    DTIE      */
                             unsigned char DTF  :2;     /*    DTF       */
                             unsigned char DTA  :1;     /*    DTA       */
                             unsigned char      :2;     /*              */
                             unsigned char DMAP :3;     /*    DMAP      */
                             }      BIT;                /*              */
                      }         DMDR;                   /*              */
                union {                                 /* DACR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char AMS  :1;     /*    AMS       */
                             unsigned char DIRS :1;     /*    DIRS      */
                             unsigned char      :3;     /*              */
                             unsigned char RPTIE:1;     /*    RPTIE     */
                             unsigned char ARS  :2;     /*    ARS       */
                             unsigned char      :2;     /*              */
                             unsigned char SAT  :2;     /*    SAT       */
                             unsigned char      :2;     /*              */
                             unsigned char DAT  :2;     /*    DAT       */
                             unsigned char SARIE:1;     /*    SARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char SARA :5;     /*    SARA      */
                             unsigned char DARIE:1;     /*    DARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char DARA :5;     /*    DARA      */
                             }      BIT;                /*              */
                      }         DACR;                   /*              */
                char            wk[260];                /*              */
                unsigned char   DMRSR;                  /* DMRSR        */
};                                                      /*              */
struct st_dmac1 {                                       /* struct DMAC1 */
                void           *DSAR;                   /* DSAR         */
                void           *DDAR;                   /* DDAR         */
                unsigned long   DOFR;                   /* DOFR         */
                unsigned long   DTCR;                   /* DTCR         */
                unsigned long   DBSR;                   /* DBSR         */
                union {                                 /* DMDR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char DTE  :1;     /*    DTE       */
                             unsigned char DACKE:1;     /*    DACKE     */
                             unsigned char TENDE:1;     /*    TENDE     */
                             unsigned char      :1;     /*              */
                             unsigned char DREQS:1;     /*    DREQS     */
                             unsigned char NRD  :1;     /*    NRD       */
                             unsigned char      :2;     /*              */
                             unsigned char ACT  :1;     /*    ACT       */
                             unsigned char      :5;     /*              */
                             unsigned char ESIF :1;     /*    ESIF      */
                             unsigned char DTIF :1;     /*    DTIF      */
                             unsigned char DTSZ :2;     /*    DTSZ      */
                             unsigned char MDS  :2;     /*    MDS       */
                             unsigned char TSEIE:1;     /*    TSEIE     */
                             unsigned char      :1;     /*              */
                             unsigned char ESIE :1;     /*    ESIE      */
                             unsigned char DTIE :1;     /*    DTIE      */
                             unsigned char DTF  :2;     /*    DTF       */
                             unsigned char DTA  :1;     /*    DTA       */
                             unsigned char      :2;     /*              */
                             unsigned char DMAP :3;     /*    DMAP      */
                             }      BIT;                /*              */
                      }         DMDR;                   /*              */
                union {                                 /* DACR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char AMS  :1;     /*    AMS       */
                             unsigned char DIRS :1;     /*    DIRS      */
                             unsigned char      :3;     /*              */
                             unsigned char RPTIE:1;     /*    RPTIE     */
                             unsigned char ARS  :2;     /*    ARS       */
                             unsigned char      :2;     /*              */
                             unsigned char SAT  :2;     /*    SAT       */
                             unsigned char      :2;     /*              */
                             unsigned char DAT  :2;     /*    DAT       */
                             unsigned char SARIE:1;     /*    SARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char SARA :5;     /*    SARA      */
                             unsigned char DARIE:1;     /*    DARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char DARA :5;     /*    DARA      */
                             }      BIT;                /*              */
                      }         DACR;                   /*              */
                char            wk[229];                /*              */
                unsigned char   DMRSR;                  /* DMRSR        */
};                                                      /*              */
struct st_dmac2 {                                       /* struct DMAC2 */
                void           *DSAR;                   /* DSAR         */
                void           *DDAR;                   /* DDAR         */
                unsigned long   DOFR;                   /* DOFR         */
                unsigned long   DTCR;                   /* DTCR         */
                unsigned long   DBSR;                   /* DBSR         */
                union {                                 /* DMDR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char DTE  :1;     /*    DTE       */
                             unsigned char DACKE:1;     /*    DACKE     */
                             unsigned char TENDE:1;     /*    TENDE     */
                             unsigned char      :1;     /*              */
                             unsigned char DREQS:1;     /*    DREQS     */
                             unsigned char NRD  :1;     /*    NRD       */
                             unsigned char      :2;     /*              */
                             unsigned char ACT  :1;     /*    ACT       */
                             unsigned char      :5;     /*              */
                             unsigned char ESIF :1;     /*    ESIF      */
                             unsigned char DTIF :1;     /*    DTIF      */
                             unsigned char DTSZ :2;     /*    DTSZ      */
                             unsigned char MDS  :2;     /*    MDS       */
                             unsigned char TSEIE:1;     /*    TSEIE     */
                             unsigned char      :1;     /*              */
                             unsigned char ESIE :1;     /*    ESIE      */
                             unsigned char DTIE :1;     /*    DTIE      */
                             unsigned char DTF  :2;     /*    DTF       */
                             unsigned char DTA  :1;     /*    DTA       */
                             unsigned char      :2;     /*              */
                             unsigned char DMAP :3;     /*    DMAP      */
                             }      BIT;                /*              */
                      }         DMDR;                   /*              */
                union {                                 /* DACR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char AMS  :1;     /*    AMS       */
                             unsigned char DIRS :1;     /*    DIRS      */
                             unsigned char      :3;     /*              */
                             unsigned char RPTIE:1;     /*    RPTIE     */
                             unsigned char ARS  :2;     /*    ARS       */
                             unsigned char      :2;     /*              */
                             unsigned char SAT  :2;     /*    SAT       */
                             unsigned char      :2;     /*              */
                             unsigned char DAT  :2;     /*    DAT       */
                             unsigned char SARIE:1;     /*    SARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char SARA :5;     /*    SARA      */
                             unsigned char DARIE:1;     /*    DARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char DARA :5;     /*    DARA      */
                             }      BIT;                /*              */
                      }         DACR;                   /*              */
                char            wk[198];                /*              */
                unsigned char   DMRSR;                  /* DMRSR        */
};                                                      /*              */
struct st_dmac3 {                                       /* struct DMAC3 */
                void           *DSAR;                   /* DSAR         */
                void           *DDAR;                   /* DDAR         */
                unsigned long   DOFR;                   /* DOFR         */
                unsigned long   DTCR;                   /* DTCR         */
                unsigned long   DBSR;                   /* DBSR         */
                union {                                 /* DMDR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char DTE  :1;     /*    DTE       */
                             unsigned char DACKE:1;     /*    DACKE     */
                             unsigned char TENDE:1;     /*    TENDE     */
                             unsigned char      :1;     /*              */
                             unsigned char DREQS:1;     /*    DREQS     */
                             unsigned char NRD  :1;     /*    NRD       */
                             unsigned char      :2;     /*              */
                             unsigned char ACT  :1;     /*    ACT       */
                             unsigned char      :5;     /*              */
                             unsigned char ESIF :1;     /*    ESIF      */
                             unsigned char DTIF :1;     /*    DTIF      */
                             unsigned char DTSZ :2;     /*    DTSZ      */
                             unsigned char MDS  :2;     /*    MDS       */
                             unsigned char TSEIE:1;     /*    TSEIE     */
                             unsigned char      :1;     /*              */
                             unsigned char ESIE :1;     /*    ESIE      */
                             unsigned char DTIE :1;     /*    DTIE      */
                             unsigned char DTF  :2;     /*    DTF       */
                             unsigned char DTA  :1;     /*    DTA       */
                             unsigned char      :2;     /*              */
                             unsigned char DMAP :3;     /*    DMAP      */
                             }      BIT;                /*              */
                      }         DMDR;                   /*              */
                union {                                 /* DACR         */
                      unsigned long LONG;               /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned char AMS  :1;     /*    AMS       */
                             unsigned char DIRS :1;     /*    DIRS      */
                             unsigned char      :3;     /*              */
                             unsigned char RPTIE:1;     /*    RPTIE     */
                             unsigned char ARS  :2;     /*    ARS       */
                             unsigned char      :2;     /*              */
                             unsigned char SAT  :2;     /*    SAT       */
                             unsigned char      :2;     /*              */
                             unsigned char DAT  :2;     /*    DAT       */
                             unsigned char SARIE:1;     /*    SARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char SARA :5;     /*    SARA      */
                             unsigned char DARIE:1;     /*    DARIE     */
                             unsigned char      :2;     /*              */
                             unsigned char DARA :5;     /*    DARA      */
                             }      BIT;                /*              */
                      }         DACR;                   /*              */
                char            wk[167];                /*              */
                unsigned char   DMRSR;                  /* DMRSR        */
};                                                      /*              */
struct st_bsc {                                         /* struct BSC   */
              union {                                   /* ABWCR        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ABWH7:1;       /*    ABWH7     */
                           unsigned char ABWH6:1;       /*    ABWH6     */
                           unsigned char ABWH5:1;       /*    ABWH5     */
                           unsigned char ABWH4:1;       /*    ABWH4     */
                           unsigned char ABWH3:1;       /*    ABWH3     */
                           unsigned char ABWH2:1;       /*    ABWH2     */
                           unsigned char ABWH1:1;       /*    ABWH1     */
                           unsigned char ABWH0:1;       /*    ABWH0     */
                           unsigned char ABWL7:1;       /*    ABWL7     */
                           unsigned char ABWL6:1;       /*    ABWL6     */
                           unsigned char ABWL5:1;       /*    ABWL5     */
                           unsigned char ABWL4:1;       /*    ABWL4     */
                           unsigned char ABWL3:1;       /*    ABWL3     */
                           unsigned char ABWL2:1;       /*    ABWL2     */
                           unsigned char ABWL1:1;       /*    ABWL1     */
                           unsigned char ABWL0:1;       /*    ABWL0     */
                           }     BIT;                   /*              */
                    }           ABWCR;                  /*              */
              union {                                   /* ASTCR        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char AST7:1;        /*    AST7      */
                           unsigned char AST6:1;        /*    AST6      */
                           unsigned char AST5:1;        /*    AST5      */
                           unsigned char AST4:1;        /*    AST4      */
                           unsigned char AST3:1;        /*    AST3      */
                           unsigned char AST2:1;        /*    AST2      */
                           unsigned char AST1:1;        /*    AST1      */
                           unsigned char AST0:1;        /*    AST0      */
                           }     BIT;                   /*              */
                    }           ASTCR;                  /*              */
              union {                                   /* WTCRA        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char   :1;          /*              */
                           unsigned char W7:3;          /*    W7        */
                           unsigned char   :1;          /*              */
                           unsigned char W6:3;          /*    W6        */
                           unsigned char   :1;          /*              */
                           unsigned char W5:3;          /*    W5        */
                           unsigned char   :1;          /*              */
                           unsigned char W4:3;          /*    W4        */
                           }     BIT;                   /*              */
                    }           WTCRA;                  /*              */
              union {                                   /* WTCRB        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char   :1;          /*              */
                           unsigned char W3:3;          /*    W3        */
                           unsigned char   :1;          /*              */
                           unsigned char W2:3;          /*    W2        */
                           unsigned char   :1;          /*              */
                           unsigned char W1:3;          /*    W1        */
                           unsigned char   :1;          /*              */
                           unsigned char W0:3;          /*    W0        */
                           }     BIT;                   /*              */
                    }           WTCRB;                  /*              */
              union {                                   /* RDNCR        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char RDN7:1;        /*    RDN7      */
                           unsigned char RDN6:1;        /*    RDN6      */
                           unsigned char RDN5:1;        /*    RDN5      */
                           unsigned char RDN4:1;        /*    RDN4      */
                           unsigned char RDN3:1;        /*    RDN3      */
                           unsigned char RDN2:1;        /*    RDN2      */
                           unsigned char RDN1:1;        /*    RDN1      */
                           unsigned char RDN0:1;        /*    RDN0      */
                           }     BIT;                   /*              */
                    }           RDNCR;                  /*              */
              union {                                   /* CSACR        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CSXH7:1;       /*    CSXH7     */
                           unsigned char CSXH6:1;       /*    CSXH6     */
                           unsigned char CSXH5:1;       /*    CSXH5     */
                           unsigned char CSXH4:1;       /*    CSXH4     */
                           unsigned char CSXH3:1;       /*    CSXH3     */
                           unsigned char CSXH2:1;       /*    CSXH2     */
                           unsigned char CSXH1:1;       /*    CSXH1     */
                           unsigned char CSXH0:1;       /*    CSXH0     */
                           unsigned char CSXT7:1;       /*    CSXT7     */
                           unsigned char CSXT6:1;       /*    CSXT6     */
                           unsigned char CSXT5:1;       /*    CSXT5     */
                           unsigned char CSXT4:1;       /*    CSXT4     */
                           unsigned char CSXT3:1;       /*    CSXT3     */
                           unsigned char CSXT2:1;       /*    CSXT2     */
                           unsigned char CSXT1:1;       /*    CSXT1     */
                           unsigned char CSXT0:1;       /*    CSXT0     */
                           }     BIT;                   /*              */
                    }           CSACR;                  /*              */
              union {                                   /* IDLCR        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char IDLS3  :1;     /*    IDLS3     */
                           unsigned char IDLS2  :1;     /*    IDLS2     */
                           unsigned char IDLS1  :1;     /*    IDLS1     */
                           unsigned char IDLS0  :1;     /*    IDLS0     */
                           unsigned char IDLCB  :2;     /*    IDLCB     */
                           unsigned char IDLCA  :2;     /*    IDLCA     */
                           unsigned char IDLSEL7:1;     /*    IDLSEL7   */
                           unsigned char IDLSEL6:1;     /*    IDLSEL6   */
                           unsigned char IDLSEL5:1;     /*    IDLSEL5   */
                           unsigned char IDLSEL4:1;     /*    IDLSEL4   */
                           unsigned char IDLSEL3:1;     /*    IDLSEL3   */
                           unsigned char IDLSEL2:1;     /*    IDLSEL2   */
                           unsigned char IDLSEL1:1;     /*    IDLSEL1   */
                           unsigned char IDLSEL0:1;     /*    IDLSEL0   */
                           }     BIT;                   /*              */
                    }           IDLCR;                  /*              */
              union {                                   /* BCR1         */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char BRLE  :1;      /*    BRLE      */
                           unsigned char BREQOE:1;      /*    BREQOE    */
                           unsigned char       :4;      /*              */
                           unsigned char WDBE  :1;      /*    WDBE      */
                           unsigned char WAITE :1;      /*    WAITE     */
                           unsigned char DKC   :1;      /*    DKC       */
                           }     BIT;                   /*              */
                    }           BCR1;                   /*              */
              union {                                   /* BCR2         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :3;       /*              */
                           unsigned char IBCCS:1;       /*    IBCCS     */
                           unsigned char      :3;       /*              */
                           unsigned char PWDBE:1;       /*    PWDBE     */
                           }      BIT;                  /*              */
                    }           BCR2;                   /*              */
              union {                                   /* ENDIANCR     */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char LE7:1;         /*    LE7       */
                           unsigned char LE6:1;         /*    LE6       */
                           unsigned char LE5:1;         /*    LE5       */
                           unsigned char LE4:1;         /*    LE4       */
                           unsigned char LE3:1;         /*    LE3       */
                           unsigned char LE2:1;         /*    LE2       */
                           }      BIT;                  /*              */
                    }           ENDIANCR;               /*              */
              char              wk[2];                  /*              */
              union {                                   /* SRAMCR       */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char BCSEL7:1;      /*    BCSEL7    */
                           unsigned char BCSEL6:1;      /*    BCSEL6    */
                           unsigned char BCSEL5:1;      /*    BCSEL5    */
                           unsigned char BCSEL4:1;      /*    BCSEL4    */
                           unsigned char BCSEL3:1;      /*    BCSEL3    */
                           unsigned char BCSEL2:1;      /*    BCSEL2    */
                           unsigned char BCSEL1:1;      /*    BCSEL1    */
                           unsigned char BCSEL0:1;      /*    BCSEL0    */
                           }     BIT;                   /*              */
                    }           SRAMCR;                 /*              */
              union {                                   /* BROMCR       */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char BSRM0:1;       /*    BSRM0     */
                           unsigned char BSTS0:3;       /*    BSTS0     */
                           unsigned char      :2;       /*              */
                           unsigned char BSWD0:2;       /*    BSWD0     */
                           unsigned char BSRM1:1;       /*    BSRM1     */
                           unsigned char BSTS1:3;       /*    BSTS1     */
                           unsigned char      :2;       /*              */
                           unsigned char BSWD1:2;       /*    BSWD1     */
                           }     BIT;                   /*              */
                    }           BROMCR;                 /*              */
              union {                                   /* MPXCR        */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char MPXE7:1;       /*    MPXE7     */
                           unsigned char MPXE6:1;       /*    MPXE6     */
                           unsigned char MPXE5:1;       /*    MPXE5     */
                           unsigned char MPXE4:1;       /*    MPXE4     */
                           unsigned char MPXE3:1;       /*    MPXE3     */
                           unsigned char      :3;       /*              */
                           unsigned char      :7;       /*              */
                           unsigned char ADDEX:1;       /*    ADDEX     */
                           }     BIT;                   /*              */
                    }           MPXCR;                  /*              */
};                                                      /*              */
union un_mdcr {                                         /* union MDCR   */
              unsigned int WORD;                        /*  Word Access */
              struct {                                  /*  Bit  Access */
                     unsigned char    :4;               /*              */
                     unsigned char MDS:4;               /*    MDS       */
                     }      BIT;                        /*              */
};                                                      /*              */
union un_syscr {                                        /* union SYSCR  */
               unsigned int WORD;                       /*  Word Access */
               struct {                                 /*  Bit  Access */
                      unsigned char        :2;          /*              */
                      unsigned char MACS   :1;          /*    MACS      */
                      unsigned char        :1;          /*              */
                      unsigned char FETCHMD:1;          /*    FETCHMD   */
                      unsigned char        :1;          /*              */
                      unsigned char EXPE   :1;          /*    EXPE      */
                      unsigned char RAME   :1;          /*    RAME      */
                      unsigned char        :6;          /*              */
                      unsigned char DTCMD  :1;          /*    DTCMD     */
                      }      BIT;                       /*              */
};                                                      /*              */
union un_sckcr {                                        /* union SCKCR  */
               unsigned int WORD;                       /*  Word Access */
               struct {                                 /*  Bit  Access */
                      unsigned char PSTOP1:1;           /*    PSTOP1    */
                      unsigned char       :1;           /*              */
                      unsigned char POSEL1:1;           /*    POSEL1    */
                      unsigned char       :2;           /*              */
                      unsigned char ICK   :3;           /*    ICK       */
                      unsigned char       :1;           /*              */
                      unsigned char PCK   :3;           /*    PCK       */
                      unsigned char       :1;           /*              */
                      unsigned char BCK   :3;           /*    BCK       */
                      }      BIT;                       /*              */
};                                                      /*              */
union un_sbycr {                                        /* union SBYCR  */
               unsigned int WORD;                       /*  Word Access */
               struct {                                 /*  Bit  Access */
                      unsigned char SSBY :1;            /*    SSBY      */
                      unsigned char OPE  :1;            /*    OPE       */
                      unsigned char      :1;            /*              */
                      unsigned char STS  :5;            /*    STS       */
                      unsigned char SLPIE:1;            /*    SLPIE     */
                      }      BIT;                       /*              */
};                                                      /*              */
struct st_mstp {                                        /* struct MSTP  */
               union {                                  /* CRA          */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char ACSE  :1;     /*    ACSE      */
                            unsigned char       :1;     /*              */
                            unsigned char _DMAC :1;     /*    DMAC      */
                            unsigned char _DTC  :1;     /*    DTC       */
                            unsigned char       :2;     /*              */
                            unsigned char _TMR23:1;     /*    TMR2,TMR3 */
                            unsigned char _TMR01:1;     /*    TMR0,TMR1 */
                            unsigned char       :2;     /*              */
                            unsigned char _DA   :1;     /*    D/A       */
                            unsigned char       :1;     /*              */
                            unsigned char _AD   :1;     /*    A/D       */
                            unsigned char       :2;     /*              */
                            unsigned char _TPU  :1;     /*    TPU       */
                            }     BIT;                  /*              */
                     }          CRA;                    /*              */
               union {                                  /* CRB          */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char _PPG  :1;     /*    PPG       */
                            unsigned char       :2;     /*              */
                            unsigned char _SCI4 :1;     /*    SCI4      */
                            unsigned char       :1;     /*              */
                            unsigned char _SCI2 :1;     /*    SCI2      */
                            unsigned char _SCI1 :1;     /*    SCI1      */
                            unsigned char _SCI0 :1;     /*    SCI0      */
                            unsigned char _IIC21:1;     /*    IIC21     */
                            unsigned char _IIC20:1;     /*    IIC20     */
                            }     BIT;                  /*              */
                     }          CRB;                    /*              */
               union {                                  /* CRC          */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char _SCI5 :1;     /*    SCI5      */
                            unsigned char _SCI6 :1;     /*    SCI6      */
                            unsigned char _TMR45:1;     /*    TMR4,TMR5 */
                            unsigned char _TMR67:1;     /*    TMR6,TMR7 */
                            unsigned char _USB  :1;     /*    USB       */
                            unsigned char _CRC  :1;     /*    CRC       */
                            unsigned char       :2;     /*              */
                            unsigned char       :3;     /*              */
                            unsigned char _RAM4 :1;     /*    RAM4      */
                            unsigned char _RAM3 :1;     /*    RAM3      */
                            unsigned char _RAM2 :1;     /*    RAM2      */
                            unsigned char _RAM1 :1;     /*    RAM1      */
                            unsigned char _RAM0 :1;     /*    RAM0      */
                            }     BIT;                  /*              */
                     }          _CRC;                   /*              */
};                                                      /*              */
struct st_dtc {                                         /* struct DTC   */
              void             *DTCVBR;                 /* DTCVBR       */
              char              wk1[412];               /*              */
              union {                                   /* DTCERA       */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char IRQ0 :1;       /*    IRQ0      */
                           unsigned char IRQ1 :1;       /*    IRQ1      */
                           unsigned char IRQ2 :1;       /*    IRQ2      */
                           unsigned char IRQ3 :1;       /*    IRQ3      */
                           unsigned char IRQ4 :1;       /*    IRQ4      */
                           unsigned char IRQ5 :1;       /*    IRQ5      */
                           unsigned char IRQ6 :1;       /*    IRQ6      */
                           unsigned char IRQ7 :1;       /*    IRQ7      */
                           unsigned char IRQ8 :1;       /*    IRQ8      */
                           unsigned char IRQ9 :1;       /*    IRQ9      */
                           unsigned char IRQ10:1;       /*    IRQ10     */
                           unsigned char IRQ11:1;       /*    IRQ11     */
                           }     BIT;                   /*              */
                    }           DTCERA;                 /*              */
              union {                                   /* DTCERB       */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ADI  :1;       /*    ADI       */
                           unsigned char      :1;       /*              */
                           unsigned char TGI0A:1;       /*    TGI0A     */
                           unsigned char TGI0B:1;       /*    TGI0B     */
                           unsigned char TGI0C:1;       /*    TGI0C     */
                           unsigned char TGI0D:1;       /*    TGI0D     */
                           unsigned char TGI1A:1;       /*    TGI1A     */
                           unsigned char TGI1B:1;       /*    TGI1B     */
                           unsigned char TGI2A:1;       /*    TGI2A     */
                           unsigned char TGI2B:1;       /*    TGI2B     */
                           unsigned char TGI3A:1;       /*    TGI3A     */
                           unsigned char TGI3B:1;       /*    TGI3B     */
                           unsigned char TGI3C:1;       /*    TGI3C     */
                           unsigned char TGI3D:1;       /*    TGI3D     */
                           unsigned char TGI4A:1;       /*    TGI4A     */
                           unsigned char TGI4B:1;       /*    TGI4B     */
                           }      BIT;                  /*              */
                    }           DTCERB;                 /*              */
              union {                                   /* DTCERC       */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TGI5A  :1;     /*    TGI5A     */
                           unsigned char TGI5B  :1;     /*    TGI5B     */
                           unsigned char CMIA0  :1;     /*    CMIA0     */
                           unsigned char CMIB0  :1;     /*    CMIB0     */
                           unsigned char CMIA1  :1;     /*    CMIA1     */
                           unsigned char CMIB1  :1;     /*    CMIB1     */
                           unsigned char CMIA2  :1;     /*    CMIA2     */
                           unsigned char CMIB2  :1;     /*    CMIB2     */
                           unsigned char CMIA3  :1;     /*    CMIA3     */
                           unsigned char CMIB3  :1;     /*    CMIB3     */
                           unsigned char DMTEND0:1;     /*    DMTEND0   */
                           unsigned char DMTEND1:1;     /*    DMTEND1   */
                           unsigned char DMTEND2:1;     /*    DMTEND2   */
                           unsigned char DMTEND3:1;     /*    DMTEND3   */
                           }      BIT;                  /*              */
                    }           DTCERC;                 /*              */
              union {                                   /* DTCERD       */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char        :2;     /*              */
                           unsigned char DMEEND0:1;     /*    DMEEND0   */
                           unsigned char DMEEND1:1;     /*    DMEEND1   */
                           unsigned char DMEEND2:1;     /*    DMEEND2   */
                           unsigned char DMEEND3:1;     /*    DMEEND3   */
                           unsigned char        :2;     /*              */
                           unsigned char        :2;     /*              */
                           unsigned char RXI0   :1;     /*    RXI0      */
                           unsigned char TXI0   :1;     /*    TXI0      */
                           unsigned char RXI1   :1;     /*    RXI1      */
                           unsigned char TXI1   :1;     /*    TXI1      */
                           unsigned char RXI2   :1;     /*    RXI2      */
                           unsigned char TXI2   :1;     /*    TXI2      */
                           }      BIT;                  /*              */
                    }           DTCERD;                 /*              */
              union {                                   /* DTCERE       */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :2;        /*              */
                           unsigned char RXI4:1;        /*    RXI4      */
                           unsigned char TXI4:1;        /*    TXI4      */
                           }      BIT;                  /*              */
                    }           DTCERE;                 /*              */
              char              wk2[6];                 /*              */
              union {                                   /* DTCCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :3;       /*              */
                           unsigned char RRS  :1;       /*    RRS       */
                           unsigned char RCHNE:1;       /*    RCHNE     */
                           unsigned char      :2;       /*              */
                           unsigned char ERR  :1;       /*    ERR       */
                           }      BIT;                  /*              */
                    }           DTCCR;                  /*              */
};                                                      /*              */
struct st_flash {                                       /* struct FLASH */
                union {                                 /* RAMER        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :4;      /*              */
                             unsigned char RAMS:1;      /*    RAMS      */
                             unsigned char RAM :3;      /*    RAM       */
                             }      BIT;                /*              */
                      }         RAMER;                  /*              */
                char            wk1[73];               /*              */
                union {                                 /* FCCS         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :3;      /*              */
                             unsigned char FLER:1;      /*    FLER      */
                             unsigned char     :3;      /*              */
                             unsigned char SCO :1;      /*    SCO       */
                             }      BIT;                /*              */
                      }         FCCS;                   /*              */
                union {                                 /* FPCS         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :7;      /*              */
                             unsigned char PPVS:1;      /*    PPVS      */
                             }      BIT;                /*              */
                      }         FPCS;                   /*              */
                union {                                 /* FECS         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :7;      /*              */
                             unsigned char EPVB:1;      /*    EPVB      */
                             }      BIT;                /*              */
                      }         FECS;                   /*              */
                char            wk2;                    /*              */
                unsigned char   FKEY;                   /* FKEY         */
                char            wk3;                    /*              */
                union {                                 /* FTDAR        */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char TDER:1;      /*    TDER      */
                             unsigned char TDA :7;      /*    TDA       */
                             }      BIT;                /*              */
                      }         FTDAR;                  /*              */
};                                                      /*              */
struct st_iic2 {                                        /* struct IIC2  */
               union {                                  /* ICCRA        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char ICE :1;       /*    ICE       */
                            unsigned char RCVD:1;       /*    RCVD      */
                            unsigned char MST :1;       /*    MST       */
                            unsigned char TRS :1;       /*    TRS       */
                            unsigned char CKS :4;       /*    CKS       */
                            }      BIT;                 /*              */
                     }          ICCRA;                  /*              */
               union {                                  /* ICCRB        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char BBSY  :1;     /*    BBSY      */
                            unsigned char SCP   :1;     /*    SCP       */
                            unsigned char SDAO  :1;     /*    SDAO      */
                            unsigned char       :1;     /*              */
                            unsigned char SCLO  :1;     /*    SCLO      */
                            unsigned char       :1;     /*              */
                            unsigned char IICRST:1;     /*    IICRST    */
                            }      BIT;                 /*              */
                     }          ICCRB;                  /*              */
               union {                                  /* ICMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :1;       /*              */
                            unsigned char WAIT:1;       /*    WAIT      */
                            unsigned char     :2;       /*              */
                            unsigned char BCWP:1;       /*    BCWP      */
                            unsigned char BC  :3;       /*    BC        */
                            }      BIT;                 /*              */
                     }          ICMR;                   /*              */
               union {                                  /* ICIER        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE  :1;      /*    TIE       */
                            unsigned char TEIE :1;      /*    TEIE      */
                            unsigned char RIE  :1;      /*    RIE       */
                            unsigned char NAKIE:1;      /*    NAKIE     */
                            unsigned char STIE :1;      /*    STIE      */
                            unsigned char ACKE :1;      /*    ACKE      */
                            unsigned char ACKBR:1;      /*    ACKBR     */
                            unsigned char ACKBT:1;      /*    ACKBT     */
                            }      BIT;                 /*              */
                     }          ICIER;                  /*              */
               union {                                  /* ICSR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TDRE :1;      /*    TDRE      */
                            unsigned char TEND :1;      /*    TEND      */
                            unsigned char RDRF :1;      /*    RDRF      */
                            unsigned char NACKF:1;      /*    NACKF     */
                            unsigned char STOP :1;      /*    STOP      */
                            unsigned char AL   :1;      /*    AL        */
                            unsigned char AAS  :1;      /*    AAS       */
                            unsigned char ADZ  :1;      /*    ADZ       */
                            }      BIT;                 /*              */
                     }          ICSR;                   /*              */
               union {                                  /* SAR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char SVA:7;        /*    SVA       */
                            }      BIT;                 /*              */
                     }          SAR;                    /*              */
               unsigned char    ICDRT;                  /* ICDRT        */
               unsigned char    ICDRR;                  /* ICDRR        */
};                                                      /*              */
struct st_da {                                          /* struct D/A   */
             unsigned char      DADR0;                  /* DADR0        */
             unsigned char      DADR1;                  /* DADR1        */
             union {                                    /* DACR01       */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char DAOE1:1;        /*    DAOE1     */
                          unsigned char DAOE0:1;        /*    DAOE0     */
                          unsigned char DAE  :1;        /*    DAE       */
                          }      BIT;                   /*              */
                   }            DACR01;                 /*              */
};                                                      /*              */
struct st_ppg {                                         /* struct PPG   */
              union {                                   /* PCR          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char G3CMS:2;       /*    G3CMS     */
                           unsigned char G2CMS:2;       /*    G2CMS     */
                           unsigned char G1CMS:2;       /*    G1CMS     */
                           unsigned char G0CMS:2;       /*    G0CMS     */
                           }      BIT;                  /*              */
                    }           PCR;                    /*              */
              union {                                   /* PMR          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char G3INV:1;       /*    G3INV     */
                           unsigned char G2INV:1;       /*    G2INV     */
                           unsigned char G1INV:1;       /*    G1INV     */
                           unsigned char G0INV:1;       /*    G0INV     */
                           unsigned char G3NOV:1;       /*    G3NOV     */
                           unsigned char G2NOV:1;       /*    G2NOV     */
                           unsigned char G1NOV:1;       /*    G1NOV     */
                           unsigned char G0NOV:1;       /*    G0NOV     */
                           }      BIT;                  /*              */
                    }           PMR;                    /*              */
              union {                                   /* NDER         */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char H;             /*    NDERH     */
                           unsigned char L;             /*    NDERL     */
                           }     BYTE;                  /*              */
                    struct {                            /*  Bit  Access */
                           unsigned char B15:1;         /*    NDER15    */
                           unsigned char B14:1;         /*    NDER14    */
                           unsigned char B13:1;         /*    NDER13    */
                           unsigned char B12:1;         /*    NDER12    */
                           unsigned char B11:1;         /*    NDER11    */
                           unsigned char B10:1;         /*    NDER10    */
                           unsigned char B9 :1;         /*    NDER9     */
                           unsigned char B8 :1;         /*    NDER8     */
                           unsigned char B7 :1;         /*    NDER7     */
                           unsigned char B6 :1;         /*    NDER6     */
                           unsigned char B5 :1;         /*    NDER5     */
                           unsigned char B4 :1;         /*    NDER4     */
                           unsigned char B3 :1;         /*    NDER3     */
                           unsigned char B2 :1;         /*    NDER2     */
                           unsigned char B1 :1;         /*    NDER1     */
                           unsigned char B0 :1;         /*    NDER0     */
                           }      BIT;                  /*              */
                    }           NDER;                   /*              */
              union {                                   /* PODR         */
                    unsigned int WORD;                  /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char H;             /*    PODRH     */
                           unsigned char L;             /*    PODRL     */
                           }     BYTE;                  /*              */
                    struct {                            /*  Bit  Access */
                           unsigned char B15:1;         /*    POD15     */
                           unsigned char B14:1;         /*    POD14     */
                           unsigned char B13:1;         /*    POD13     */
                           unsigned char B12:1;         /*    POD12     */
                           unsigned char B11:1;         /*    POD11     */
                           unsigned char B10:1;         /*    POD10     */
                           unsigned char B9 :1;         /*    POD9      */
                           unsigned char B8 :1;         /*    POD8      */
                           unsigned char B7 :1;         /*    POD7      */
                           unsigned char B6 :1;         /*    POD6      */
                           unsigned char B5 :1;         /*    POD5      */
                           unsigned char B4 :1;         /*    POD4      */
                           unsigned char B3 :1;         /*    POD3      */
                           unsigned char B2 :1;         /*    POD2      */
                           unsigned char B1 :1;         /*    POD1      */
                           unsigned char B0 :1;         /*    POD0      */
                           }     BIT;                   /*              */
                    }           PODR;                   /*              */
              union {                                   /* NDRH (H'7C)  */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char B15:1;         /*    NDR15     */
                           unsigned char B14:1;         /*    NDR14     */
                           unsigned char B13:1;         /*    NDR13     */
                           unsigned char B12:1;         /*    NDR12     */
                           unsigned char B11:1;         /*    NDR11     */
                           unsigned char B10:1;         /*    NDR10     */
                           unsigned char B9 :1;         /*    NDR9      */
                           unsigned char B8 :1;         /*    NDR8      */
                           }      BIT;                  /*              */
                    }           NDRH1;                  /*              */
              union {                                   /* NDRL (H'7D)  */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char B7:1;          /*    NDR7      */
                           unsigned char B6:1;          /*    NDR6      */
                           unsigned char B5:1;          /*    NDR5      */
                           unsigned char B4:1;          /*    NDR4      */
                           unsigned char B3:1;          /*    NDR3      */
                           unsigned char B2:1;          /*    NDR2      */
                           unsigned char B1:1;          /*    NDR1      */
                           unsigned char B0:1;          /*    NDR0      */
                           }      BIT;                  /*              */
                    }           NDRL1;                  /*              */
              union {                                   /* NDRH (H'7E)  */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :4;         /*              */
                           unsigned char B11:1;         /*    NDR11     */
                           unsigned char B10:1;         /*    NDR10     */
                           unsigned char B9 :1;         /*    NDR9      */
                           unsigned char B8 :1;         /*    NDR8      */
                           }      BIT;                  /*              */
                    }           NDRH2;                  /*              */
              union {                                   /* NDRL (H'7F)  */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char   :4;          /*              */
                           unsigned char B3:1;          /*    NDR3      */
                           unsigned char B2:1;          /*    NDR2      */
                           unsigned char B1:1;          /*    NDR1      */
                           unsigned char B0:1;          /*    NDR0      */
                           }      BIT;                  /*              */
                    }           NDRL2;                  /*              */
};                                                      /*              */
struct st_crc {                                         /* struct CRC   */
              union {                                   /* CRCCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit Access  */
                           unsigned char DORCLR:1;      /*    DORCLR    */
                           unsigned char       :4;      /*              */
                           unsigned char LMS   :1;      /*    LMS       */
                           unsigned char G     :2;      /*    G         */
                           }      BIT;                  /*              */
                    }           CRCCR;                  /*              */
              unsigned char     CRCDIR;                 /* CRCDIR       */
              unsigned int      CRCDOR;                 /* CRCDOR       */
};                                                      /*              */
struct st_sci {                                         /* struct SCI   */
              union {                                   /* SMR          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CA  :1;        /*    C/A       */
                           unsigned char CHR :1;        /*    CHR       */
                           unsigned char _PE :1;        /*    PE        */
                           unsigned char OE  :1;        /*    O/E       */
                           unsigned char STOP:1;        /*    STOP      */
                           unsigned char MP  :1;        /*    MP        */
                           unsigned char CKS :2;        /*    CKS       */
                           }      BIT;                  /*              */
                    }           SMR;                    /*              */
              unsigned char     BRR;                    /* BRR          */
              union {                                   /* SCR          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TIE :1;        /*    TIE       */
                           unsigned char RIE :1;        /*    RIE       */
                           unsigned char TE  :1;        /*    TE        */
                           unsigned char RE  :1;        /*    RE        */
                           unsigned char MPIE:1;        /*    MPIE      */
                           unsigned char TEIE:1;        /*    TEIE      */
                           unsigned char CKE :2;        /*    CKE       */
                           }      BIT;                  /*              */
                    }           SCR;                    /*              */
              unsigned char     TDR;                    /* TDR          */
              union {                                   /* SSR          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TDRE:1;        /*    TDRE      */
                           unsigned char RDRF:1;        /*    RDRF      */
                           unsigned char ORER:1;        /*    ORER      */
                           unsigned char FER :1;        /*    FER       */
                           unsigned char PER :1;        /*    PER       */
                           unsigned char TEND:1;        /*    TEND      */
                           unsigned char MPB :1;        /*    MPB       */
                           unsigned char MPBT:1;        /*    MPBT      */
                           }      BIT;                  /*              */
                    }           SSR;                    /*              */
              unsigned char     RDR;                    /* RDR          */
              union {                                   /* SCMR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :4;        /*              */
                           unsigned char SDIR:1;        /*    SDIR      */
                           unsigned char SINV:1;        /*    SINV      */
                           unsigned char     :1;        /*              */
                           unsigned char SMIF:1;        /*    SMIF      */
                           }      BIT;                  /*              */
                    }           SCMR;                   /*              */
};                                                      /*              */
struct st_sci2 {                                        /* struct SCI2  */
               union {                                  /* SEMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :4;       /*              */
                            unsigned char ABCS:1;       /*    ABCS      */
                            unsigned char ACS :3;       /*    ACS       */
                            }      BIT;                 /*              */
                     }          SEMR;                   /*              */
               char             wk[219];                /*              */
               union {                                  /* SMR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CA  :1;       /*    C/A       */
                            unsigned char CHR :1;       /*    CHR       */
                            unsigned char _PE :1;       /*    PE        */
                            unsigned char OE  :1;       /*    O/E       */
                            unsigned char STOP:1;       /*    STOP      */
                            unsigned char MP  :1;       /*    MP        */
                            unsigned char CKS :2;       /*    CKS       */
                            }      BIT;                 /*              */
                     }          SMR;                    /*              */
               unsigned char    BRR;                    /* BRR          */
               union {                                  /* SCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE :1;       /*    TIE       */
                            unsigned char RIE :1;       /*    RIE       */
                            unsigned char TE  :1;       /*    TE        */
                            unsigned char RE  :1;       /*    RE        */
                            unsigned char MPIE:1;       /*    MPIE      */
                            unsigned char TEIE:1;       /*    TEIE      */
                            unsigned char CKE :2;       /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCR;                    /*              */
               unsigned char    TDR;                    /* TDR          */
               union {                                  /* SSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TDRE:1;       /*    TDRE      */
                            unsigned char RDRF:1;       /*    RDRF      */
                            unsigned char ORER:1;       /*    ORER      */
                            unsigned char FER :1;       /*    FER       */
                            unsigned char PER :1;       /*    PER       */
                            unsigned char TEND:1;       /*    TEND      */
                            unsigned char MPB :1;       /*    MPB       */
                            unsigned char MPBT:1;       /*    MPBT      */
                            }      BIT;                 /*              */
                     }          SSR;                    /*              */
               unsigned char    RDR;                    /* RDR          */
               union {                                  /* SCMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :4;       /*              */
                            unsigned char SDIR:1;       /*    SDIR      */
                            unsigned char SINV:1;       /*    SINV      */
                            unsigned char     :1;       /*              */
                            unsigned char SMIF:1;       /*    SMIF      */
                            }      BIT;                 /*              */
                     }          SCMR;                   /*              */
};                                                      /*              */
struct st_sci5 {                                        /* struct SCI5  */
               union {                                  /* SMR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CA  :1;       /*    C/A       */
                            unsigned char CHR :1;       /*    CHR       */
                            unsigned char _PE :1;       /*    PE        */
                            unsigned char OE  :1;       /*    O/E       */
                            unsigned char STOP:1;       /*    STOP      */
                            unsigned char MP  :1;       /*    MP        */
                            unsigned char CKS :2;       /*    CKS       */
                            }      BIT;                 /*              */
                     }          SMR;                    /*              */
               unsigned char    BRR;                    /* BRR          */
               union {                                  /* SCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE :1;       /*    TIE       */
                            unsigned char RIE :1;       /*    RIE       */
                            unsigned char TE  :1;       /*    TE        */
                            unsigned char RE  :1;       /*    RE        */
                            unsigned char MPIE:1;       /*    MPIE      */
                            unsigned char TEIE:1;       /*    TEIE      */
                            unsigned char CKE :2;       /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCR;                    /*              */
               unsigned char    TDR;                    /* TDR          */
               union {                                  /* SSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TDRE:1;       /*    TDRE      */
                            unsigned char RDRF:1;       /*    RDRF      */
                            unsigned char ORER:1;       /*    ORER      */
                            unsigned char FER :1;       /*    FER       */
                            unsigned char PER :1;       /*    PER       */
                            unsigned char TEND:1;       /*    TEND      */
                            unsigned char MPB :1;       /*    MPB       */
                            unsigned char MPBT:1;       /*    MPBT      */
                            }      BIT;                 /*              */
                     }          SSR;                    /*              */
               unsigned char    RDR;                    /* RDR          */
               union {                                  /* SCMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :4;       /*              */
                            unsigned char SDIR:1;       /*    SDIR      */
                            unsigned char SINV:1;       /*    SINV      */
                            unsigned char     :1;       /*              */
                            unsigned char SMIF:1;       /*    SMIF      */
                            }      BIT;                 /*              */
                     }          SCMR;                   /*              */
               char             wk1;                    /*              */
               union {                                  /* SEMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :3;       /*              */
                            unsigned char ABCS:1;       /*    ABCS      */
                            unsigned char ACS :4;       /*    ACS       */
                            }      BIT;                 /*              */
                     }          SEMR;                   /*              */
               char             wk2[3];                 /*              */
               union {                                  /* IrCR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char IrE    :1;    /*    IrE       */
                            unsigned char IrCKS  :3;    /*    IrCKS     */
                            unsigned char IrTxINV:1;    /*    IrTxINV   */
                            unsigned char IrRxINV:1;    /*    IrRxINV   */
                            }      BIT;                 /*              */
                     }          IrCR;                   /*              */
};                                                      /*              */
struct st_sci6 {                                        /* struct SCI6  */
               union {                                  /* SMR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CA  :1;       /*    C/A       */
                            unsigned char CHR :1;       /*    CHR       */
                            unsigned char _PE :1;       /*    PE        */
                            unsigned char OE  :1;       /*    O/E       */
                            unsigned char STOP:1;       /*    STOP      */
                            unsigned char MP  :1;       /*    MP        */
                            unsigned char CKS :2;       /*    CKS       */
                            }      BIT;                 /*              */
                     }          SMR;                    /*              */
               unsigned char    BRR;                    /* BRR          */
               union {                                  /* SCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE :1;       /*    TIE       */
                            unsigned char RIE :1;       /*    RIE       */
                            unsigned char TE  :1;       /*    TE        */
                            unsigned char RE  :1;       /*    RE        */
                            unsigned char MPIE:1;       /*    MPIE      */
                            unsigned char TEIE:1;       /*    TEIE      */
                            unsigned char CKE :2;       /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCR;                    /*              */
               unsigned char    TDR;                    /* TDR          */
               union {                                  /* SSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TDRE:1;       /*    TDRE      */
                            unsigned char RDRF:1;       /*    RDRF      */
                            unsigned char ORER:1;       /*    ORER      */
                            unsigned char FER :1;       /*    FER       */
                            unsigned char PER :1;       /*    PER       */
                            unsigned char TEND:1;       /*    TEND      */
                            unsigned char MPB :1;       /*    MPB       */
                            unsigned char MPBT:1;       /*    MPBT      */
                            }      BIT;                 /*              */
                     }          SSR;                    /*              */
               unsigned char    RDR;                    /* RDR          */
               union {                                  /* SCMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :4;       /*              */
                            unsigned char SDIR:1;       /*    SDIR      */
                            unsigned char SINV:1;       /*    SINV      */
                            unsigned char     :1;       /*              */
                            unsigned char SMIF:1;       /*    SMIF      */
                            }      BIT;                 /*              */
                     }          SCMR;                   /*              */
               char             wk;                     /*              */
               union {                                  /* SEMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :3;       /*              */
                            unsigned char ABCS:1;       /*    ABCS      */
                            unsigned char ACS :4;       /*    ACS       */
                            }      BIT;                 /*              */
                     }          SEMR;                   /*              */
};                                                      /*              */
struct st_smci {                                        /* struct SMCI  */
               union {                                  /* SMR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char GM :1;        /*    GM        */
                            unsigned char BLK:1;        /*    BLK       */
                            unsigned char _PE:1;        /*    PE        */
                            unsigned char OE :1;        /*    O/E       */
                            unsigned char BCP:2;        /*    BCP       */
                            unsigned char CKS:2;        /*    CKS       */
                            }      BIT;                 /*              */
                     }          SMR;                    /*              */
               unsigned char    BRR;                    /* BRR          */
               union {                                  /* SCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE :1;       /*    TIE       */
                            unsigned char RIE :1;       /*    RIE       */
                            unsigned char TE  :1;       /*    TE        */
                            unsigned char RE  :1;       /*    RE        */
                            unsigned char MPIE:1;       /*    MPIE      */
                            unsigned char TEIE:1;       /*    TEIE      */
                            unsigned char CKE :2;       /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCR;                    /*              */
               unsigned char    TDR;                    /* TDR          */
               union {                                  /* SSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TDRE:1;       /*    TDRE      */
                            unsigned char RDRF:1;       /*    RDRF      */
                            unsigned char ORER:1;       /*    ORER      */
                            unsigned char ERS :1;       /*    ERS       */
                            unsigned char PER :1;       /*    PER       */
                            unsigned char TEND:1;       /*    TEND      */
                            unsigned char MPB :1;       /*    MPB       */
                            unsigned char MPBT:1;       /*    MPBT      */
                            }      BIT;                 /*              */
                     }          SSR;                    /*              */
               unsigned char    RDR;                    /* RDR          */
               union {                                  /* SCMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :4;       /*              */
                            unsigned char SDIR:1;       /*    SDIR      */
                            unsigned char SINV:1;       /*    SINV      */
                            unsigned char     :1;       /*              */
                            unsigned char SMIF:1;       /*    SMIF      */
                            }      BIT;                 /*              */
                     }          SCMR;                   /*              */
};                                                      /*              */
struct st_smci2 {                                       /* struct SMCI2 */
                union {                                 /* SEMR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :4;      /*              */
                             unsigned char ABCS:1;      /*    ABCS      */
                             unsigned char ACS :3;      /*    ACS       */
                             }      BIT;                /*              */
                      }         SEMR;                   /*              */
                char            wk[219];                /*              */
                union {                                 /* SMR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char GM :1;       /*    GM        */
                             unsigned char BLK:1;       /*    BLK       */
                             unsigned char _PE:1;       /*    PE        */
                             unsigned char OE :1;       /*    O/E       */
                             unsigned char BCP:2;       /*    BCP       */
                             unsigned char CKS:2;       /*    CKS       */
                             }      BIT;                /*              */
                      }         SMR;                    /*              */
                unsigned char   BRR;                    /* BRR          */
                union {                                 /* SCR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char TIE :1;      /*    TIE       */
                             unsigned char RIE :1;      /*    RIE       */
                             unsigned char TE  :1;      /*    TE        */
                             unsigned char RE  :1;      /*    RE        */
                             unsigned char MPIE:1;      /*    MPIE      */
                             unsigned char TEIE:1;      /*    TEIE      */
                             unsigned char CKE :2;      /*    CKE       */
                             }      BIT;                /*              */
                      }         SCR;                    /*              */
                unsigned char   TDR;                    /* TDR          */
                union {                                 /* SSR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char TDRE:1;      /*    TDRE      */
                             unsigned char RDRF:1;      /*    RDRF      */
                             unsigned char ORER:1;      /*    ORER      */
                             unsigned char ERS :1;      /*    ERS       */
                             unsigned char PER :1;      /*    PER       */
                             unsigned char TEND:1;      /*    TEND      */
                             unsigned char MPB :1;      /*    MPB       */
                             unsigned char MPBT:1;      /*    MPBT      */
                             }      BIT;                /*              */
                      }         SSR;                    /*              */
                unsigned char   RDR;                    /* RDR          */
                union {                                 /* SCMR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :4;      /*              */
                             unsigned char SDIR:1;      /*    SDIR      */
                             unsigned char SINV:1;      /*    SINV      */
                             unsigned char     :1;      /*              */
                             unsigned char SMIF:1;      /*    SMIF      */
                             }      BIT;                /*              */
                      }         SCMR;                   /*              */
};                                                      /*              */
struct st_smci5 {                                       /* struct SMCI5 */
                union {                                 /* SMR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char GM :1;       /*    GM        */
                             unsigned char BLK:1;       /*    BLK       */
                             unsigned char _PE:1;       /*    PE        */
                             unsigned char OE :1;       /*    O/E       */
                             unsigned char BCP:2;       /*    BCP       */
                             unsigned char CKS:2;       /*    CKS       */
                             }      BIT;                /*              */
                      }         SMR;                    /*              */
                unsigned char   BRR;                    /* BRR          */
                union {                                 /* SCR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char TIE :1;      /*    TIE       */
                             unsigned char RIE :1;      /*    RIE       */
                             unsigned char TE  :1;      /*    TE        */
                             unsigned char RE  :1;      /*    RE        */
                             unsigned char MPIE:1;      /*    MPIE      */
                             unsigned char TEIE:1;      /*    TEIE      */
                             unsigned char CKE :2;      /*    CKE       */
                             }      BIT;                /*              */
                      }         SCR;                    /*              */
                unsigned char   TDR;                    /* TDR          */
                union {                                 /* SSR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char TDRE:1;      /*    TDRE      */
                             unsigned char RDRF:1;      /*    RDRF      */
                             unsigned char ORER:1;      /*    ORER      */
                             unsigned char ERS :1;      /*    ERS       */
                             unsigned char PER :1;      /*    PER       */
                             unsigned char TEND:1;      /*    TEND      */
                             unsigned char MPB :1;      /*    MPB       */
                             unsigned char MPBT:1;      /*    MPBT      */
                             }      BIT;                /*              */
                      }         SSR;                    /*              */
                unsigned char   RDR;                    /* RDR          */
                union {                                 /* SCMR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :4;      /*              */
                             unsigned char SDIR:1;      /*    SDIR      */
                             unsigned char SINV:1;      /*    SINV      */
                             unsigned char     :1;      /*              */
                             unsigned char SMIF:1;      /*    SMIF      */
                             }      BIT;                /*              */
                      }         SCMR;                   /*              */
               char             wk1;                    /*              */
               union {                                  /* SEMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :3;       /*              */
                            unsigned char ABCS:1;       /*    ABCS      */
                            unsigned char ACS :4;       /*    ACS       */
                            }      BIT;                 /*              */
                     }          SEMR;                   /*              */
               char             wk2[3];                 /*              */
               union {                                  /* IrCR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char IrE    :1;    /*    IrE       */
                            unsigned char IrCKS  :3;    /*    IrCKS     */
                            unsigned char IrTxINV:1;    /*    IrTxINV   */
                            unsigned char IrRxINV:1;    /*    IrRxINV   */
                            }      BIT;                 /*              */
                     }          IrCR;                   /*              */
};                                                      /*              */
struct st_smci6 {                                       /* struct SMCI6 */
                union {                                 /* SMR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char GM :1;       /*    GM        */
                             unsigned char BLK:1;       /*    BLK       */
                             unsigned char _PE:1;       /*    PE        */
                             unsigned char OE :1;       /*    O/E       */
                             unsigned char BCP:2;       /*    BCP       */
                             unsigned char CKS:2;       /*    CKS       */
                             }      BIT;                /*              */
                      }         SMR;                    /*              */
                unsigned char   BRR;                    /* BRR          */
                union {                                 /* SCR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char TIE :1;      /*    TIE       */
                             unsigned char RIE :1;      /*    RIE       */
                             unsigned char TE  :1;      /*    TE        */
                             unsigned char RE  :1;      /*    RE        */
                             unsigned char MPIE:1;      /*    MPIE      */
                             unsigned char TEIE:1;      /*    TEIE      */
                             unsigned char CKE :2;      /*    CKE       */
                             }      BIT;                /*              */
                      }         SCR;                    /*              */
                unsigned char   TDR;                    /* TDR          */
                union {                                 /* SSR          */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char TDRE:1;      /*    TDRE      */
                             unsigned char RDRF:1;      /*    RDRF      */
                             unsigned char ORER:1;      /*    ORER      */
                             unsigned char ERS :1;      /*    ERS       */
                             unsigned char PER :1;      /*    PER       */
                             unsigned char TEND:1;      /*    TEND      */
                             unsigned char MPB :1;      /*    MPB       */
                             unsigned char MPBT:1;      /*    MPBT      */
                             }      BIT;                /*              */
                      }         SSR;                    /*              */
                unsigned char   RDR;                    /* RDR          */
                union {                                 /* SCMR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char     :4;      /*              */
                             unsigned char SDIR:1;      /*    SDIR      */
                             unsigned char SINV:1;      /*    SINV      */
                             unsigned char     :1;      /*              */
                             unsigned char SMIF:1;      /*    SMIF      */
                             }      BIT;                /*              */
                      }         SCMR;                   /*              */
               char             wk;                     /*              */
               union {                                  /* SEMR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :3;       /*              */
                            unsigned char ABCS:1;       /*    ABCS      */
                            unsigned char ACS :4;       /*    ACS       */
                            }      BIT;                 /*              */
                     }          SEMR;                   /*              */
};                                                      /*              */
struct st_ad {                                          /* struct A/D   */
             unsigned int       ADDRA;                  /* ADDRA        */
             unsigned int       ADDRB;                  /* ADDRB        */
             unsigned int       ADDRC;                  /* ADDRC        */
             unsigned int       ADDRD;                  /* ADDRD        */
             unsigned int       ADDRE;                  /* ADDRE        */
             unsigned int       ADDRF;                  /* ADDRF        */
             unsigned int       ADDRG;                  /* ADDRG        */
             unsigned int       ADDRH;                  /* ADDRH        */
             union {                                    /* ADCSR        */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char ADF :1;         /*    ADF       */
                          unsigned char ADIE:1;         /*    ADIE      */
                          unsigned char ADST:1;         /*    ADST      */
                          unsigned char     :1;         /*              */
                          unsigned char CH  :4;         /*    CH        */
                          }      BIT;                   /*              */
                   }            ADCSR;                  /*              */
             union {                                    /* ADCR         */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char TRGS :2;        /*    TRGS      */
                          unsigned char SCANE:1;        /*    SCANE     */
                          unsigned char SCANS:1;        /*    SCANS     */
                          unsigned char CKS  :2;        /*    CKS       */
                          }      BIT;                   /*              */
                   }            ADCR;                   /*              */
};                                                      /*              */
union un_wdt {                                          /* union WDT    */
             struct {                                   /* Read  Access */
                    union {                             /* TCSR         */
                          unsigned char BYTE;           /*  Byte Access */
                          struct {                      /*  Bit  Access */
                                 unsigned char OVF :1;  /*    OVF       */
                                 unsigned char WTIT:1;  /*    WT/IT     */
                                 unsigned char TME :1;  /*    TME       */
                                 unsigned char     :2;  /*              */
                                 unsigned char CKS :3;  /*    CKS       */
                                 }      BIT;            /*              */
                          }       TCSR;                 /*              */
                    unsigned char TCNT;                 /* TCNT         */
                    char          wk;                   /*              */
                    union {                             /* RSTCSR       */
                          unsigned char BYTE;           /*  Byte Access */
                          struct {                      /*              */
                                 unsigned char WOVF:1;  /*    WOVF      */
                                 unsigned char RSTE:1;  /*    RSTE      */
                                 }      BIT;            /*              */
                          }       RSTCSR;               /*              */
                    } READ;                             /*              */
             struct {                                   /* Write Access */
                    unsigned int  TCSR;                 /* TCSR/TCNT    */
                    unsigned int  RSTCSR;               /* RSTCSR       */
                    } WRITE;                            /*              */
};                                                      /*              */
struct st_tmra {                                        /* struct TMRA  */
               union {                                  /* TCR0         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR0;                   /*              */
               union {                                  /* TCR1         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR1;                   /*              */
               union {                                  /* TCSR0        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char ADTE:1;       /*    ADTE      */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR0;                  /*              */
               union {                                  /* TCSR1        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char     :1;       /*              */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR1;                  /*              */
               unsigned int     TCORA;                  /* TCORA        */
               unsigned int     TCORB;                  /* TCORB        */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCCR0        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR0;                  /*              */
               union {                                  /* TCCR1        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR1;                  /*              */
};                                                      /*              */
struct st_tmrb {                                        /* struct TMRB  */
               union {                                  /* TCR2         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR2;                   /*              */
               union {                                  /* TCR3         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR3;                   /*              */
               union {                                  /* TCSR2        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char ADTE:1;       /*    ADTE      */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR2;                  /*              */
               union {                                  /* TCSR3        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char     :1;       /*              */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR3;                  /*              */
               unsigned int     TCORA;                  /* TCORA        */
               unsigned int     TCORB;                  /* TCORB        */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCCR2        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR2;                  /*              */
               union {                                  /* TCCR3        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR3;                  /*              */
};                                                      /*              */
struct st_tmrc {                                        /* struct TMRC  */
               union {                                  /* TCR4         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR4;                   /*              */
               union {                                  /* TCR5         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR5;                   /*              */
               union {                                  /* TCSR4        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char ADTE:1;       /*    ADTE      */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR4;                  /*              */
               union {                                  /* TCSR5        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char     :1;       /*              */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR5;                  /*              */
               unsigned int     TCORA;                  /* TCORA        */
               unsigned int     TCORB;                  /* TCORB        */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCCR4        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR4;                  /*              */
               union {                                  /* TCCR5        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR5;                  /*              */
};                                                      /*              */
struct st_tmrd {                                        /* struct TMRD  */
               union {                                  /* TCR6         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR6;                   /*              */
               union {                                  /* TCR7         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR7;                   /*              */
               union {                                  /* TCSR6        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char ADTE:1;       /*    ADTE      */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR6;                  /*              */
               union {                                  /* TCSR7        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char     :1;       /*              */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR7;                  /*              */
               unsigned int     TCORA;                  /* TCORA        */
               unsigned int     TCORB;                  /* TCORB        */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCCR6        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR6;                  /*              */
               union {                                  /* TCCR7        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR7;                  /*              */
};                                                      /*              */
struct st_tmr0 {                                        /* struct TMR0  */
               union {                                  /* TCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR;                    /*              */
               char             wk1;                    /*              */
               union {                                  /* TCSR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char ADTE:1;       /*    ADTE      */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR;                   /*              */
               char             wk2;                    /*              */
               unsigned char    TCORA;                  /* TCORA        */
               char             wk3;                    /*              */
               unsigned char    TCORB;                  /* TCORB        */
               char             wk4;                    /*              */
               unsigned char    TCNT;                   /* TCNT         */
               char             wk5;                    /*              */
               union {                                  /* TCCR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR;                   /*              */
};                                                      /*              */
struct st_tmr1 {                                        /* struct TMR1  */
               union {                                  /* TCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMIEB:1;      /*    CMIEB     */
                            unsigned char CMIEA:1;      /*    CMIEA     */
                            unsigned char OVIE :1;      /*    OVIE      */
                            unsigned char CCLR :2;      /*    CCLR      */
                            unsigned char CKS  :3;      /*    CKS       */
                            }      BIT;                 /*              */
                     }          TCR;                    /*              */
               char             wk1;                    /*              */
               union {                                  /* TCSR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CMFB:1;       /*    CMFB      */
                            unsigned char CMFA:1;       /*    CMFA      */
                            unsigned char OVF :1;       /*    OVF       */
                            unsigned char     :1;       /*              */
                            unsigned char OS  :4;       /*    OS        */
                            }      BIT;                 /*              */
                     }          TCSR;                   /*              */
               char             wk2;                    /*              */
               unsigned char    TCORA;                  /* TCORA        */
               char             wk3;                    /*              */
               unsigned char    TCORB;                  /* TCORB        */
               char             wk4;                    /*              */
               unsigned char    TCNT;                   /* TCNT         */
               char             wk5;                    /*              */
               union {                                  /* TCCR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char TMRIS:1;      /*    TMRIS     */
                            unsigned char      :1;      /*              */
                            unsigned char ICKS :2;      /*    ICKS      */
                            }      BIT;                 /*              */
                     }          TCCR;                   /*              */
};                                                      /*              */
struct st_tpu {                                         /* struct TPU   */
              union {                                   /* TSTR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :2;        /*              */
                           unsigned char CST5:1;        /*    CST5      */
                           unsigned char CST4:1;        /*    CST4      */
                           unsigned char CST3:1;        /*    CST3      */
                           unsigned char CST2:1;        /*    CST2      */
                           unsigned char CST1:1;        /*    CST1      */
                           unsigned char CST0:1;        /*    CST0      */
                           }      BIT;                  /*              */
                    }           TSTR;                   /*              */
              union {                                   /* TSYR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :2;       /*              */
                           unsigned char SYNC5:1;       /*    SYNC5     */
                           unsigned char SYNC4:1;       /*    SYNC4     */
                           unsigned char SYNC3:1;       /*    SYNC3     */
                           unsigned char SYNC2:1;       /*    SYNC2     */
                           unsigned char SYNC1:1;       /*    SYNC1     */
                           unsigned char SYNC0:1;       /*    SYNC0     */
                           }      BIT;                  /*              */
                    }           TSYR;                   /*              */
};                                                      /*              */
struct st_tpu0 {                                        /* struct TPU0  */
               union {                                  /* TCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CCLR:3;       /*    CCLR      */
                            unsigned char CKEG:2;       /*    CKEG      */
                            unsigned char TPSC:3;       /*    TPSC      */
                            }      BIT;                 /*              */
                     }          TCR;                    /*              */
               union {                                  /* TMDR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char    :2;        /*              */
                            unsigned char BFB:1;        /*    BFB       */
                            unsigned char BFA:1;        /*    BFA       */
                            unsigned char MD :4;        /*    MD        */
                            }      BIT;                 /*              */
                     }          TMDR;                   /*              */
               union {                                  /* TIOR         */
                     unsigned int WORD;                 /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char H;            /*    TIORH     */
                            unsigned char L;            /*    TIORL     */
                            }     BYTE;                 /*              */
                     struct {                           /*  Bit  Access */
                            unsigned char IOB:4;        /*    IOB       */
                            unsigned char IOA:4;        /*    IOA       */
                            unsigned char IOD:4;        /*    IOD       */
                            unsigned char IOC:4;        /*    IOC       */
                            }     BIT;                  /*              */
                     }          TIOR;                   /*              */
               union {                                  /* TIER         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TTGE :1;      /*    TTGE      */
                            unsigned char      :2;      /*              */
                            unsigned char TCIEV:1;      /*    TCIEV     */
                            unsigned char TGIED:1;      /*    TGIED     */
                            unsigned char TGIEC:1;      /*    TGIEC     */
                            unsigned char TGIEB:1;      /*    TGIEB     */
                            unsigned char TGIEA:1;      /*    TGIEA     */
                            }      BIT;                 /*              */
                     }          TIER;                   /*              */
               union {                                  /* TSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :3;       /*              */
                            unsigned char TCFV:1;       /*    TCFV      */
                            unsigned char TGFD:1;       /*    TGFD      */
                            unsigned char TGFC:1;       /*    TGFC      */
                            unsigned char TGFB:1;       /*    TGFB      */
                            unsigned char TGFA:1;       /*    TGFA      */
                            }      BIT;                 /*              */
                     }          TSR;                    /*              */
               unsigned int     TCNT;                   /* TCNT         */
               unsigned int     TGRA;                   /* TGRA         */
               unsigned int     TGRB;                   /* TGRB         */
               unsigned int     TGRC;                   /* TGRC         */
               unsigned int     TGRD;                   /* TGRD         */
};                                                      /*              */
struct st_tpu1 {                                        /* struct TPU1  */
               union {                                  /* TCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :1;       /*              */
                            unsigned char CCLR:2;       /*    CCLR      */
                            unsigned char CKEG:2;       /*    CKEG      */
                            unsigned char TPSC:3;       /*    TPSC      */
                            }      BIT;                 /*              */
                     }          TCR;                    /*              */
               union {                                  /* TMDR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char   :4;         /*              */
                            unsigned char MD:4;         /*    MD        */
                            }      BIT;                 /*              */
                     }          TMDR;                   /*              */
               union {                                  /* TIOR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char IOB:4;        /*    IOB       */
                            unsigned char IOA:4;        /*    IOA       */
                            }      BIT;                 /*              */
                     }          TIOR;                   /*              */
               char             wk;                     /*              */
               union {                                  /* TIER         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TTGE :1;      /*    TTGE      */
                            unsigned char      :1;      /*              */
                            unsigned char TCIEU:1;      /*    TCIEU     */
                            unsigned char TCIEV:1;      /*    TCIEV     */
                            unsigned char      :2;      /*              */
                            unsigned char TGIEB:1;      /*    TGIEB     */
                            unsigned char TGIEA:1;      /*    TGIEA     */
                            }      BIT;                 /*              */
                     }          TIER;                   /*              */
               union {                                  /* TSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TCFD:1;       /*    TCFD      */
                            unsigned char     :1;       /*              */
                            unsigned char TCFU:1;       /*    TCFU      */
                            unsigned char TCFV:1;       /*    TCFV      */
                            unsigned char     :2;       /*              */
                            unsigned char TGFB:1;       /*    TGFB      */
                            unsigned char TGFA:1;       /*    TGFA      */
                            }      BIT;                 /*              */
                     }          TSR;                    /*              */
               unsigned int     TCNT;                   /* TCNT         */
               unsigned int     TGRA;                   /* TGRA         */
               unsigned int     TGRB;                   /* TGRB         */
};                                                      /*              */
#define USB     (*(volatile struct st_usb   *)0xFFEE00) /* USB   Address*/
#define P1      (*(volatile struct st_p1    *)0xFFFB80) /* P1    Address*/
#define P2      (*(volatile struct st_p2    *)0xFFFB81) /* P2    Address*/
#define P5      (*(volatile struct st_p5    *)0xFFFB94) /* P5    Address*/
#define P6      (*(volatile struct st_p6    *)0xFFFB85) /* P6    Address*/
#define PA      (*(volatile struct st_pa    *)0xFFFB89) /* PA    Address*/
#define PB      (*(volatile struct st_pb    *)0xFFFB8A) /* PB    Address*/
#define PD      (*(volatile struct st_pd    *)0xFFFB8C) /* PD    Address*/
#define PE      (*(volatile struct st_pe    *)0xFFFB8D) /* PE    Address*/
#define PF      (*(volatile struct st_pf    *)0xFFFB8E) /* PF    Address*/
#define PH      (*(volatile struct st_ph    *)0xFFFBA0) /* PH    Address*/
#define PI      (*(volatile struct st_pi    *)0xFFFBA1) /* PI    Address*/
#define PM      (*(volatile struct st_pm    *)0xFFEE50) /* PM    Address*/
#define PFC     (*(volatile struct st_pfc   *)0xFFFBC0) /* PFC   Address*/
#define INTC    (*(volatile struct st_intc  *)0xFFFBCE) /* INTC  Address*/
#define DMAC0   (*(volatile struct st_dmac0 *)0xFFFC00) /* DMAC0 Address*/
#define DMAC1   (*(volatile struct st_dmac1 *)0xFFFC20) /* DMAC1 Address*/
#define DMAC2   (*(volatile struct st_dmac2 *)0xFFFC40) /* DMAC2 Address*/
#define DMAC3   (*(volatile struct st_dmac3 *)0xFFFC60) /* DMAC3 Address*/
#define BSC     (*(volatile struct st_bsc   *)0xFFFD84) /* BSC   Address*/
#define MDCR    (*(volatile union  un_mdcr  *)0xFFFDC0) /* MDCR  Address*/
#define SYSCR   (*(volatile union  un_syscr *)0xFFFDC2) /* SYSCR Address*/
#define SCKCR   (*(volatile union  un_sckcr *)0xFFFDC4) /* SCKCR Address*/
#define SBYCR   (*(volatile union  un_sbycr *)0xFFFDC6) /* SBYCR Address*/
#define MSTP    (*(volatile struct st_mstp  *)0xFFFDC8) /* MSTP  Address*/
#define DTC     (*(volatile struct st_dtc   *)0xFFFD80) /* DTC   Address*/
#define FLASH   (*(volatile struct st_flash *)0xFFFD9E) /* FLASH Address*/
#define IIC20   (*(volatile struct st_iic2  *)0xFFFEB0) /* IIC20 Address*/
#define IIC21   (*(volatile struct st_iic2  *)0xFFFEB8) /* IIC21 Address*/
#define DA      (*(volatile struct st_da    *)0xFFFF68) /* D/A   Address*/
#define PPG     (*(volatile struct st_ppg   *)0xFFFF76) /* PPG   Address*/
#define CRC     (*(volatile struct st_crc   *)0xFFEA4C) /* CRC   Address*/
#define SCI0    (*(volatile struct st_sci   *)0xFFFF80) /* SCI0  Address*/
#define SCI1    (*(volatile struct st_sci   *)0xFFFF88) /* SCI1  Address*/
#define SCI2    (*(volatile struct st_sci2  *)0xFFFE84) /* SCI2  Address*/
#define SCI4    (*(volatile struct st_sci   *)0xFFFE90) /* SCI4  Address*/
#define SCI5    (*(volatile struct st_sci5  *)0xFFF600) /* SCI5  Address*/
#define SCI6    (*(volatile struct st_sci6  *)0xFFF610) /* SCI6  Address*/
#define SMCI0   (*(volatile struct st_smci  *)0xFFFF80) /* SMCI0 Address*/
#define SMCI1   (*(volatile struct st_smci  *)0xFFFF88) /* SMCI1 Address*/
#define SMCI2   (*(volatile struct st_smci2 *)0xFFFE84) /* SMCI2 Address*/
#define SMCI4   (*(volatile struct st_smci  *)0xFFFE90) /* SMCI4 Address*/
#define SMCI5   (*(volatile struct st_smci5 *)0xFFF600) /* SMCI5 Address*/
#define SMCI6   (*(volatile struct st_smci6 *)0xFFF610) /* SMCI6 Address*/
#define AD      (*(volatile struct st_ad    *)0xFFFF90) /* A/D   Address*/
#define WDT     (*(volatile union  un_wdt   *)0xFFFFA4) /* WDT   Address*/
#define TMRA    (*(volatile struct st_tmra  *)0xFFFFB0) /* TMRA  Address*/
#define TMRB    (*(volatile struct st_tmrb  *)0xFFFEC0) /* TMRB  Address*/
#define TMRC    (*(volatile struct st_tmrc  *)0xFFEA40) /* TMRC  Address*/
#define TMRD    (*(volatile struct st_tmrd  *)0xFFEA50) /* TMRD  Address*/
#define TMR0    (*(volatile struct st_tmr0  *)0xFFFFB0) /* TMR0  Address*/
#define TMR1    (*(volatile struct st_tmr1  *)0xFFFFB1) /* TMR1  Address*/
#define TMR2    (*(volatile struct st_tmr0  *)0xFFFEC0) /* TMR2  Address*/
#define TMR3    (*(volatile struct st_tmr1  *)0xFFFEC1) /* TMR3  Address*/
#define TMR4    (*(volatile struct st_tmr0  *)0xFFEA40) /* TMR4  Address*/
#define TMR5    (*(volatile struct st_tmr1  *)0xFFEA41) /* TMR5  Address*/
#define TMR6    (*(volatile struct st_tmr0  *)0xFFEA50) /* TMR6  Address*/
#define TMR7    (*(volatile struct st_tmr1  *)0xFFEA51) /* TMR7  Address*/
#define TPU     (*(volatile struct st_tpu   *)0xFFFFBC) /* TPU   Address*/
#define TPU0    (*(volatile struct st_tpu0  *)0xFFFFC0) /* TPU0  Address*/
#define TPU1    (*(volatile struct st_tpu1  *)0xFFFFD0) /* TPU1  Address*/
#define TPU2    (*(volatile struct st_tpu1  *)0xFFFFE0) /* TPU2  Address*/
#define TPU3    (*(volatile struct st_tpu0  *)0xFFFFF0) /* TPU3  Address*/
#define TPU4    (*(volatile struct st_tpu1  *)0xFFFEE0) /* TPU4  Address*/
#define TPU5    (*(volatile struct st_tpu1  *)0xFFFEF0) /* TPU5  Address*/
