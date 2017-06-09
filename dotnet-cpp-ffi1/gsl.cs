class GSL
{
	[DllImport("libgsl.so")]
	static extern  double gsl_sf_bessel_J0(double x);

	[DllImport("libgsl.so")]
	static extern double gsl_complex_abs( complex z);

	[DllImport("libgsl.so")]
	static extern complex gsl_complex_sin (complex z);


	public struct complex
	{
		[MarshalAs(UnmanagedType.ByValArray, SizeConst=2)]
		public double[] dat;

		complex(double re, double im)
		{
			this.dat=new double[] {re,im};
		}
	}
	
	
	public static void test()
	{
	Console.WriteLine(gsl_sf_bessel_J0(0.0));

	complex c1;
	c1.dat=new double[] {3,4};
	Console.WriteLine(gsl_complex_abs(c1));

	complex c2;
	c2=gsl_complex_sin(c1);



	Console.WriteLine(c1.dat[0]);
	//Console.WriteLine(c2.dat[0]);


	}


}
