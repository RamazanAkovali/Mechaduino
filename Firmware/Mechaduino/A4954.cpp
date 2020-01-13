#include "A4954.h"
#include "board.h"
#include "Configuration.h"
#include "Configurationals.h"
#include "analogFastWrite.h"
#include "State.h"
#include <stdint.h>



void output(int electric_angle, int effort) {
  static int32_t v_ref_coil_A;
  static int32_t v_ref_coil_B;

  static int32_t pole_angle;

  int16_t sine;
  int16_t cosine;

  if (effort != 0) {
    pole_angle = mod(-(phase_multiplier * electric_angle) , 3600);

    sine = sin_lookup[pole_angle];
    cosine = cos_lookup[pole_angle];

    v_ref_coil_A = (abs(effort) * sine) / 4096;
    v_ref_coil_B = (abs(effort) * cosine) / 4096;

    v_ref_coil_A = (gainCoilA * v_ref_coil_A) / 8192;
    v_ref_coil_B = (gainCoilB * v_ref_coil_B) / 8192;

    if (v_ref_coil_A > 0)  {
      digitalWriteDirect(IN_1, LOW);
      digitalWriteDirect(IN_2, HIGH);
    }
    else  {
      digitalWriteDirect(IN_1, HIGH);
      digitalWriteDirect(IN_2, LOW);
    }

    if (v_ref_coil_B > 0)  {
      digitalWriteDirect(IN_3, LOW);
      digitalWriteDirect(IN_4, HIGH);
    }
    else  {
      digitalWriteDirect(IN_3, HIGH);
      digitalWriteDirect(IN_4, LOW);
    }

    analogFastWrite(VREF_1, abs(v_ref_coil_A));
    analogFastWrite(VREF_2, abs(v_ref_coil_B));
  }
  else {
    analogFastWrite(VREF_1, 0);
    analogFastWrite(VREF_2, 0);

    digitalWriteDirect(IN_1, LOW);
    digitalWriteDirect(IN_2, LOW);
    digitalWriteDirect(IN_3, LOW);
    digitalWriteDirect(IN_4, LOW);
  }

}


