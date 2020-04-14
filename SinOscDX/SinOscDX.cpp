#include <SC_PlugIn.h>
#include <math.h>

static InterfaceTable *ft;

struct SinOscDX : public Unit {
    float m_cpstoinc, m_radtoinc;
    int32 m_lomask, m_phase;
    float m_freq, m_pm, m_fb;
    float m_pre, m_mem;
};

void SinOscDX_next_a(SinOscDX* unit, int inNumSamples) {
    float *out = OUT(0);
    float *pm = IN(1);

    float *table0 = ft->mSineWavetable;
    float *table1 = table0 + 1;

    int32 lomask = unit->m_lomask;
    int32 phase = unit->m_phase;

    float freq = unit->m_freq;
    float freq_slope = CALCSLOPE(IN0(0) * unit->m_cpstoinc, freq);

    float radtoinc = unit->m_radtoinc;

    float fb = unit->m_fb;
    float fb_slope = CALCSLOPE(IN0(2) * unit->m_radtoinc, fb);

    float cur;
    float pre = unit->m_pre;
    float mem = unit->m_mem;

    for (int i = 0; i < inNumSamples; ++i) {
        cur = lookupi1(table0, table1, phase + (int32)(pm[i] * radtoinc + (fb < 0 ? -fb * fabs(mem) : fb * mem)), lomask);
        mem = (pre + cur) * 0.5;
        pre = cur;
        out[i] = mem;
        phase += (int32)(freq);
        freq += freq_slope;
        fb += fb_slope;
    }
    unit->m_phase = phase;
    unit->m_freq = freq;
    unit->m_fb = fb;
    unit->m_pre = pre;
    unit->m_mem = mem;
}

void SinOscDX_next_k(SinOscDX* unit, int inNumSamples) {
    float *out = OUT(0);

    float *table0 = ft->mSineWavetable;
    float *table1 = table0 + 1;

    int32 lomask = unit->m_lomask;
    int32 phase = unit->m_phase;

    float freq = unit->m_freq;
    float freq_slope = CALCSLOPE(IN0(0) * unit->m_cpstoinc, freq);

    float pm = unit->m_pm;
    float pm_slope = CALCSLOPE(IN0(1) * unit->m_radtoinc, pm);

    float fb = unit->m_fb;
    float fb_slope = CALCSLOPE(IN0(2) * unit->m_radtoinc, fb);

    float cur;
    float pre = unit->m_pre;
    float mem = unit->m_mem;

    for (int i = 0; i < inNumSamples; ++i) {
        cur = lookupi1(table0, table1, phase + (int32)(pm + (fb < 0 ? -fb * fabs(mem) : fb * mem)), lomask);
        mem = (pre + cur) * 0.5;
        pre = cur;
        out[i] = mem;
        phase += (int32)(freq);
        freq += freq_slope;
        pm += pm_slope;
        fb += fb_slope;
    }
    unit->m_phase = phase;
    unit->m_freq = freq;
    unit->m_pm = pm;
    unit->m_fb = fb;
    unit->m_pre = pre;
    unit->m_mem = mem;
}

void SinOscDX_Ctor(SinOscDX* unit) {
    int size = ft->mSineSize;
    unit->m_cpstoinc = size * SAMPLEDUR * 65536.;
    unit->m_radtoinc = size * rtwopi * 65536.;
    unit->m_lomask = (size - 1) << 3;
    unit->m_freq = IN0(0) * unit->m_cpstoinc;
    unit->m_pm = IN0(1) * unit->m_radtoinc;
    unit->m_fb = IN0(2) * unit->m_radtoinc;
    unit->m_pre = 0.;
    unit->m_mem = 0.;

    if (INRATE(1) == calc_FullRate) {
        SETCALC(SinOscDX_next_a);
        unit->m_phase = 0;
    } else {
        SETCALC(SinOscDX_next_k);
        unit->m_phase = (int32)(unit->m_pm);
    }

    SinOscDX_next_k(unit, 1);
}

PluginLoad(SinOscDX)
{
    ft = inTable;
    DefineSimpleUnit(SinOscDX);
}
