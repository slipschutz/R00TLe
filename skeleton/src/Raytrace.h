const int MAPDIM = 126;
/* const int nbinthetacm = 12; */
/* //const int nscint = 48; */
/* const int nscint = 60; */
/* //const int npmt = 96; */
/* const int npmt = 120; */
/* const Double_t tick2nsec = 4.; */
/* const int nicchan = 16; */

/* const int ncalib = 10; */

class Invmap_t {
 public:
  Double_t coefficient[MAPDIM];
  Int_t x_exponent[MAPDIM];
  Int_t a_exponent[MAPDIM];
  Int_t y_exponent[MAPDIM];
  Int_t b_exponent[MAPDIM];

};


//_____________________________________________________________________________
void loadInverseMap(TString InverseMapFile,
                             Invmap_t *a, Invmap_t *y, Invmap_t *b, Invmap_t *d)
{
  // for inverse map
  Int_t    num, order, exp_x, exp_a, exp_y, exp_b, exp_l, exp_p;
  Double_t coeff;
  Int_t    kind;

  std::ifstream ifs(InverseMapFile);
  if (ifs.fail()) {
    Error(__FUNCTION__, "No such prm file %s", InverseMapFile.Data());
    exit(0);
  }

  string buf;
  // The first line is the title.
  // e.g. "S800 inverse map - Brho=2.32930 - M=3 - Q=2 - Exp. 12007"
  getline(ifs, buf);

  kind = 0;
  while (ifs && getline(ifs, buf)) {
    for (Int_t pos = 0; pos < 80; pos++) {
      if (isspace(buf[pos])) {
      
      } else if (isalpha(buf[pos])) {
	kind++;
	break;
      } else if (ispunct(buf[pos])) {
	break;
      } else if (isdigit(buf[pos])) {
	istringstream iss(buf, istringstream::in);
	iss >> num >> coeff >> order >> exp_x >> exp_a >> exp_y >> exp_b >> exp_l >> exp_p;
	if (kind == 1) {
	  a->coefficient[num] = coeff;
	  a->x_exponent[num]  = exp_x;
	  a->a_exponent[num]  = exp_a;
	  a->y_exponent[num]  = exp_y;
	  a->b_exponent[num]  = exp_b;

	} else if (kind == 2) {
	  y->coefficient[num] = coeff;
	  y->x_exponent[num]  = exp_x;
	  y->a_exponent[num]  = exp_a;
	  y->y_exponent[num]  = exp_y;
	  y->b_exponent[num]  = exp_b;
	} else if (kind == 3) {
	  b->coefficient[num] = coeff;
	  b->x_exponent[num]  = exp_x;
	  b->a_exponent[num]  = exp_a;
	  b->y_exponent[num]  = exp_y;
	  b->b_exponent[num]  = exp_b;
	} else if (kind == 4) {
	  d->coefficient[num] = coeff;
	  d->x_exponent[num]  = exp_x;
	  d->a_exponent[num]  = exp_a;
	  d->y_exponent[num]  = exp_y;
	  d->b_exponent[num]  = exp_b;
	}
	break;
      }
    }
  }
}

//_____________________________________________________________________________
void Raytracing(Invmap_t& a, Invmap_t& y, Invmap_t& b, Invmap_t& d,
		const Float_t& xf,  const Float_t& af,  const Float_t& yf,  const Float_t& bf,
                         Float_t& ai,  Float_t& yi,  Float_t& bi,  Float_t& di)
{
  // Raytracing
  // A minus sign is necessary for xfp and afp
  // (I guess this is related to the CB (change bending) command
  // in the COSY script which makes the inverse map.)
  for (Int_t inum = 0; inum < MAPDIM; inum++) {
    ai = ai
      + a.coefficient[inum]
      * TMath::Power(-xf, a.x_exponent[inum])
      * TMath::Power(-af, a.a_exponent[inum])
      * TMath::Power(yf, a.y_exponent[inum])
      * TMath::Power(bf, a.b_exponent[inum]);

    yi = yi
      + y.coefficient[inum]
      * TMath::Power(-xf, y.x_exponent[inum])
      * TMath::Power(-af, y.a_exponent[inum])
      * TMath::Power(yf, y.y_exponent[inum])
      * TMath::Power(bf, y.b_exponent[inum]);

    bi = bi
      + b.coefficient[inum]
      * TMath::Power(-xf, b.x_exponent[inum])
      * TMath::Power(-af, b.a_exponent[inum])
      * TMath::Power(yf, b.y_exponent[inum])
      * TMath::Power(bf, b.b_exponent[inum]);

    di = di
      + d.coefficient[inum]
      * TMath::Power(-xf, d.x_exponent[inum])
      * TMath::Power(-af, d.a_exponent[inum])
      * TMath::Power(yf, d.y_exponent[inum])
      * TMath::Power(bf, d.b_exponent[inum]);
  }
}
