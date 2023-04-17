#!/bin/csh

source /usr/local/sklib_gcc4.8.5/skofl_19b/env.csh

set PLOTDIR=/home/jmcelwee/monitoring/plotting
set WEBDIR=/home/calib/public_html/water_scattering/monitoring/uk_gdload/plots

yes|cp $PLOTDIR/col_totQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/col_totQ_six.png $WEBDIR/
yes|cp $PLOTDIR/col_totQ.png $WEBDIR/
yes|cp $PLOTDIR/dif_totQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_totQ_six.png $WEBDIR/
yes|cp $PLOTDIR/dif_totQ.png $WEBDIR/

yes|cp $PLOTDIR/col_spotQ.png $WEBDIR/
yes|cp $PLOTDIR/col_spotQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/col_spotQ_six.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_spotQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_spotQ_six.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_spotQ.png $WEBDIR/

yes|cp $PLOTDIR/col_monQ_all.png $WEBDIR/
yes|cp $PLOTDIR/col_monQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/col_monQ_six.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_monQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_monQ_six.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_monQ_all.png $WEBDIR/

yes|cp $PLOTDIR/col_normQ_all.png $WEBDIR/
yes|cp $PLOTDIR/col_normQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/col_normQ_six.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_normQ_day.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_normQ_six.png $WEBDIR/ 
yes|cp $PLOTDIR/dif_normQ_all.png $WEBDIR/

yes|cp $PLOTDIR/top_diffuser_ratioQ.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_ratioQ_day.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_ratioQ_six.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_totQ.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_totQ_day.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_totQ_six.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_bbQ.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_bbQ_day.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_bbQ_six.png $WEBDIR/

yes|cp $PLOTDIR/top_diffuser_bzyQ.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_botQ.png $WEBDIR/
yes|cp $PLOTDIR/top_diffuser_barQ.png $WEBDIR/
