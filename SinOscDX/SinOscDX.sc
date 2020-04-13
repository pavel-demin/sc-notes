SinOscDX : PureUGen {
	*ar {
		arg freq=440.0, pm=0.0, fb=0.0, mul=1.0, add=0.0;
		^this.multiNew('audio', freq, pm, fb).madd(mul, add)
	}
}
