/*
 * Copyright (c) 1999-2008 Mark D. Hill and David A. Wood
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "mem/ruby/filters/H3BloomFilter.hh"

#include "base/intmath.hh"
#include "base/logging.hh"

using namespace std;

static int H3[64][16] = {
    { 33268410,   395488709,  311024285,  456111753,
      181495008,  119997521,  220697869,  433891432,
      755927921,  515226970,  719448198,  349842774,
      269183649,  463275672,  429800228,  521598937,  },

    { 628677802,  820947732,  809435975,  1024657192,
      887631270,  412050215,  391365090,  324227279,
      318338329,  1038393087, 489807930,  387366128,
      518096428,  324184340,  429376066,  447109279,  },

    { 599747653,  404960623,  103933604,  946416030,
      656460913,  925957005,  1047665689, 163552053,
      88359290,   841315415,  899833584,  1067336680,
      348549994,  464045876,  270252128,  829897652,  },

    { 215495230,  966696438,  82589012,   750102795,
      909780866,  920285789,  769759214,  331966823,
      939936006,  439950703,  883794828,  1009277508,
      61634610,   741444350,  98689608,   524144422,  },

    { 93868534,   196958667,  774076619,  327921978,
      122538783,  879785030,  690748527,  3498564,
      83163077,   1027963025, 582088444,  466152216,
      312424878,  550064499,  646612667,  561099434,  },

    { 1002047931, 395477707,  821317480,  890482112,
      697094476,  263813044,  840275189,  469664185,
      795625845,  211504898,  99204277,   1004491153,
      725930417,  1064479221, 893834767,  839719181,  },

    { 278507126,  985111995,  706462983,  1042178726,
      123281719,  963778122,  500881056,  726291104,
      134293026,  568379664,  317050609,  533470307,
      1022365922, 197645211,  315125721,  634827678,  },

    { 219227366,  553960647,  870169525,  322232839,
      508322497,  648672696,  249405795,  883596102,
      476433133,  541372919,  646647793,  1042679515,
      43242483,   600187508,  499866821,  135713210,  },

    { 52837162,   96966684,   401840460,  1071661176,
      733560065,  150035417,  341319946,  811582750,
      636173904,  519054065,  196321433,  1028294565,
      882204070,  522965093,  48884074,   117810166,  },

    { 650860353,  789534698,  328813544,  473250022,
      143128306,  173196006,  846958825,  174632187,
      683273509,  405459497,  787235556,  773873501,
      240110267,  426797736,  92043842,   711789240,  },

    { 586637493,  5059646,    398035664,  6686087,
      498300175,  948278148,  681227731,  592751744,
      572019677,  558044722,  589368271,  695745538,
      1073416749, 529192035,  550984939,  1070620580, },

    { 102904663,  647598516,  758863940,  313426443,
      76504114,   1050747783, 708436441,  563815069,
      224107668,  875925186,  167675944,  926209739,
      279737287,  1040288182, 768184312,  371708956,  },

    { 683968868,  1027427757, 180781926,  742898864,
      624078545,  645659833,  577225838,  987150210,
      723410002,  224013421,  993286634,  33188488,
      247264323,  888018697,  38048664,   189037096,  },

    { 475612146,  426739285,  873726278,  529192871,
      607715202,  388486246,  987001312,  474493980,
      259747270,  417465536,  217062395,  392858482,
      563810075,  137852805,  1051814153, 72895217,   },

    { 71277086,   785496675,  500608842,  89633426,
      274085706,  248467935,  838061983,  48106147,
      773662506,  49545328,   9071573,    100739031,
      602018002,  904371654,  534132064,  332211304,  },

    { 401893602,  735125342,  775548339,  210224843,
      256081130,  482894412,  350801633,  1035713633,
      429458128,  327281409,  739927752,  359327650,
      886942880,  847691759,  752417993,  359445596,  },

    { 267472014,  1050659620, 1068232362, 1049684368,
      17130239,   690524969,  793224378,  14455158,
      423092885,  873853424,  430535778,  7867877,
      309731959,  370260786,  862353083,  403906850,  },

    { 993077283,  218812656,  389234651,  393202875,
      413116501,  263300295,  470013158,  592730725,
      441847172,  732392823,  407574059,  875664777,
      271347307,  792954404,  554774761,  1022424300, },

    { 675919719,  637054073,  784720745,  149714381,
      813144874,  502525801,  635436670,  1003196587,
      160786091,  947509775,  969788637,  26854073,
      257964369,  63898568,   539767732,  772364518,  },

    { 943076868,  1021732472, 697575075,  15843624,
      617573396,  534113303,  122953324,  964873912,
      942995378,  87830944,   1012914818, 455484661,
      592160054,  599844284,  810394353,  836812568,  },

    { 688992674,  279465370,  731582262,  687883235,
      438178468,  80493001,   342701501,  663561405,
      23360106,   531315007,  508931618,  36294623,
      231216223,  840438413,  255665680,  663205938,  },

    { 857265418,  552630887,  8173237,    792122963,
      210140052,  823124938,  667709953,  751538219,
      991957789,  462064153,  19070176,   726604748,
      714567823,  151147895,  1012619677, 697114353,  },

    { 467105652,  683256174,  702387467,  28730434,
      549942998,  48712701,   960519696,  1008345587,
      679267717,  370932249,  880419471,  352141567,
      331640403,  598772468,  95160685,   812053015,  },

    { 1053491323, 430526562,  1014938507, 109685515,
      765949103,  177288303,  1034642653, 485421658,
      71850281,   981034542,  61620389,   601367920,
      504420930,  220599168,  583051998,  158735752,  },

    { 103033901,  522494916,  658494760,  959206022,
      931348143,  834510661,  21542994,   189699884,
      679327018,  171983002,  96774168,   456133168,
      543103352,  923945936,  970074188,  643658485,  },

    { 566379913,  805798263,  840662512,  820206124,
      796507494,  223712542,  118811519,  662246595,
      809326534,  416471323,  748027186,  161169753,
      739149488,  276330378,  924837051,  964873733,  },

    { 585882743,  135502711,  3386031,    625631285,
      1068193307, 270342640,  432739484,  556606453,
      826419155,  1038540977, 158000202,  69109538,
      207087256,  298111218,  678046259,  184611498,  },

    { 305310710,  46237988,   855726974,  735975153,
      930663798,  425764232,  104362407,  391371443,
      867622101,  71645091,   61824734,   661902640,
      293738633,  309416189,  281710675,  879317360,  },

    { 398146324,  398293087,  689145387,  1038451703,
      521637478,  516134620,  314658937,  830334981,
      583400300,  340083705,  68029852,   675389876,
      994635780,  788959180,  406967042,  74403607,   },

    { 69463153,   744427484,  191639960,  590927798,
      969916795,  546846769,  728756758,  889355646,
      520855076,  136068426,  776132410,  189663815,
      252051082,  533662856,  362198652,  1026161384, },

    { 584984279,  1004834381, 568439705,  834508761,
      21812513,   670870173,  1052043300, 341868768,
      473755574,  124339439,  36193947,   437997647,
      137419489,  58705193,   337793711,  340738909,  },

    { 898051466,  512792906,  234874060,  655358775,
      683745319,  671676404,  428888546,  639928192,
      672697722,  176477579,  747020991,  758211282,
      443045009,  205395173,  1016944273, 5584717,    },

    { 156038300,  138620174,  588466825,  1061494056,
      1013672100, 1064257198, 881417791,  839470738,
      83519030,   100875683,  237486447,  461483733,
      681527127,  777996147,  574635362,  815974538,  },

    { 184168473,  519509808,  62531892,   51821173,
      43787358,   385711644,  141325169,  36069511,
      584183031,  571372909,  671503175,  226486781,
      194932686,  1045460970, 753718579,  331442433,  },

    { 73065106,   1015327221, 630916840,  1058053470,
      306737587,  296343219,  907194989,  920172546,
      224516225,  818625553,  551143849,  634570650,
      432966225,  756438259,  939564853,  767999933,  },

    { 884775648,  394862257,  446787794,  219833788,
      727195727,  728122304,  249888353,  732947974,
      289908868,  448282580,  618161877,  898939716,
      739554163,  860631799,  1058977530, 86916736,   },

    { 143850006,  352708694,  200194048,  979764914,
      629404175,  546279766,  72106714,   860980514,
      313190585,  897143111,  308425797,  953791785,
      349924906,  221457005,  950588925,  908254505,  },

    { 950032043,  829868728,  68623614,   714624605,
      69760597,   297275854,  355894016,  985369737,
      882852618,  864071289,  958512902,  950910111,
      991368991,  829645051,  434698210,  771350575,  },

    { 552695074,  319195551,  80297396,   496413831,
      944046531,  621525571,  617653363,  416729825,
      441842808,  9847464,    99420657,   1033914550,
      812966458,  937053011,  673390195,  934577365,  },

    { 1034695843, 190969665,  332900185,  51897434,
      523888639,  883512843,  146908572,  506785674,
      565814307,  692255649,  314052926,  826386588,
      430691325,  866927620,  413880214,  936474339,  },

    { 129380164,  741739952,  1013703462, 494392795,
      957214600,  1010879043, 931790677,  94551922,
      988065869,  120637871,  882506912,  395075379,
      210570485,  812422692,  910383687,  817722285,  },

    { 51850866,   283408630,  1053047202, 858940389,
      818507731,  477082181,  353546901,  993324368,
      407093779,  231608253,  1067319867, 73159811,
      429792535,  971320614,  565699344,  718823399,  },

    { 408185106,  491493570,  596050720,  310776444,
      703628192,  454438809,  523988035,  728512200,
      686012353,  976339656,  72816924,   116926720,
      165866591,  452043792,  866943072,  968545481,  },

    { 443231195,  905907843,  1061421320, 746360489,
      1043120338, 1069659155, 463359031,  688303227,
      186550710,  155347339,  1044842421, 1005904570,
      69332909,   706951903,  422513657,  882038450,  },

    { 430990623,  946501980,  742556791,  278398643,
      183759217,  659404315,  279754382,  1069347846,
      843746517,  222777670,  990835599,  548741637,
      129220580,  1392170,    1032654091, 894058935,  },

    { 452042227,  751640705,  259481376,  765824585,
      145991469,  1013683228, 1055491225, 536379588,
      392593350,  913368594,  1029429776, 226857786,
      31505342,   1054416381, 32341741,   687106649,  },

    { 404750944,  811417027,  869530820,  773491060,
      810901282,  979340397,  1036910290, 461764404,
      834235095,  765695033,  604692390,  452158120,
      928988098,  442719218,  1024059719, 167723114,  },

    { 974245177,  1046377300, 1003424287, 787349855,
      336314155,  875074696,  1018462718, 890313003,
      367376809,  86355556,   1020618772, 890710345,
      444741481,  373230261,  767064947,  840920177,  },

    { 719581124,  431808156,  138301690,  668222575,
      497413494,  740492013,  485033226,  125301442,
      831265111,  879071459,  341690480,  152975256,
      850330086,  717444507,  694225877,  785340566,  },

    { 1032766252, 140959364,  737474726,  1062767538,
      364464647,  331414723,  356152634,  642832379,
      158733632,  374691640,  285504811,  345349905,
      876599880,  476392727,  479589210,  606376325,  },

    { 174997730,  778177086,  319164313,  163614456,
      10331364,   599358958,  8331663,    237538058,
      159173957,  174533880,  65588684,   878222844,
      424467599,  901803515,  187504218,  776690353,  },

    { 803856182,  965850321,  694948067,  218315960,
      358416571,  683713254,  178069303,  428076035,
      686176454,  579553217,  357306738,  315018080,
      886852373,  568563910,  896839725,  257416821,  },

    { 401650013,  183289141,  497957228,  879734476,
      265024455,  825794561,  889237440,  323359863,
      100258491,  991414783,  313986632,  85847250,
      362520248,  276103512,  1041630342, 525981595,  },

    { 487732740,  46201705,   990837834,  62744493,
      1067364756, 58015363,   690846283,  680262648,
      997278956,  469357861,  432164624,  996763915,
      211907847,  167824295,  144928194,  454839915,  },

    { 41404232,   514493300,  259546924,  578217256,
      972345130,  123299213,  346040332,  1014668104,
      520910639,  579955198,  36627803,   179072921,
      547684341,  598950511,  269497394,  854352266,  },

    { 603906768,  100863318,  708837659,  204175569,
      375560904,  908375384,  28314106,   6303733,
      175283124,  749851198,  308667367,  415293931,
      225365403,  1032188331, 977112710,  819705229,  },

    { 399767123,  697985692,  356790426,  643687584,
      298624218,  185095167,  381653926,  876816342,
      296720023,  2205879,    235816616,  521850105,
      622753786,  1021421218, 726349744,  256504902,  },

    { 851245024,  1022500222, 511909628,  313809625,
      99776025,   39710175,   798739932,  741832408,
      140631966,  898295927,  607660421,  870669312,
      1051422478, 789055529,  669113756,  681943450,  },

    { 853872755,  491465269,  503341472,  98019440,
      258267420,  335602837,  320687824,  1053324395,
      24932389,   955011453,  934255131,  435625663,
      501568768,  238967025,  549987406,  248619780,  },

    { 411151284,  576471205,  757985419,  544137226,
      968135693,  877548443,  194586894,  74882373,
      248353663,  21207540,   273789651,  853653916,
      861267970,  533253322,  3739570,    661358586,  },

    { 271430986,  71390029,   257643671,  949329860,
      348156406,  251939238,  445808698,  48269799,
      907589462,  105677619,  635451508,  20805932,
      464874661,  7542147,    243619464,  288304568,  },

    { 368215982,  530288964,  770090421,  660961164,
      614935537,  630760399,  931299233,  794519275,
      779918979,  401746493,  561237006,  1027202224,
      258968003,  339508073,  1050610516, 1064307013, },

    { 1039172162, 448331205,  928997884,  49813151,
      198712120,  992335354,  671024050,  879525220,
      745915336,  1038822580, 138669665,  917958819,
      681422342,  792868818,  924762727,  816386174,  },

    { 515190336,  313808618,  441296783,  1022120897,
      792325033,  354387581,  59273006,   280075434,
      411357221,  665274694,  4054464,    1059046246,
      394261773,  848616745,  15446017,   517723271,  },
};

H3BloomFilter::H3BloomFilter(int size, int hashes, bool parallel)
{
    //TODO: change this ugly init code...
    primes_list[0] = 9323;
    primes_list[1] = 11279;
    primes_list[2] = 10247;
    primes_list[3] = 30637;
    primes_list[4] = 25717;
    primes_list[5] = 43711;

    mults_list[0] = 255;
    mults_list[1] = 29;
    mults_list[2] = 51;
    mults_list[3] = 3;
    mults_list[4] = 77;
    mults_list[5] = 43;

    adds_list[0] = 841;
    adds_list[1] = 627;
    adds_list[2] = 1555;
    adds_list[3] = 241;
    adds_list[4] = 7777;
    adds_list[5] = 65931;

    m_filter_size = size;
    m_num_hashes = hashes;
    isParallel = parallel;

    m_filter_size_bits = floorLog2(m_filter_size);

    m_par_filter_size = m_filter_size / m_num_hashes;
    m_par_filter_size_bits = floorLog2(m_par_filter_size);

    m_filter.resize(m_filter_size);
    clear();
}

H3BloomFilter::~H3BloomFilter()
{
}

void
H3BloomFilter::clear()
{
    for (int i = 0; i < m_filter_size; i++) {
        m_filter[i] = 0;
    }
}

void
H3BloomFilter::merge(AbstractBloomFilter *other_filter)
{
    // assumes both filters are the same size!
    H3BloomFilter * temp = (H3BloomFilter*) other_filter;
    for (int i = 0; i < m_filter_size; ++i){
        m_filter[i] |= (*temp)[i];
    }
}

void
H3BloomFilter::set(Addr addr)
{
    for (int i = 0; i < m_num_hashes; i++) {
        int idx = get_index(addr, i);
        m_filter[idx] = 1;
    }
}

void
H3BloomFilter::unset(Addr addr)
{
    panic("ERROR: Unset should never be called in a Bloom filter");
}

bool
H3BloomFilter::isSet(Addr addr)
{
    bool res = true;

    for (int i = 0; i < m_num_hashes; i++) {
        int idx = get_index(addr, i);
        res = res && m_filter[idx];
    }
    return res;
}

int
H3BloomFilter::getCount(Addr addr)
{
    return isSet(addr)? 1: 0;
}

int
H3BloomFilter::getIndex(Addr addr)
{
    return 0;
}

int
H3BloomFilter::readBit(const int index)
{
    return 0;
}

void
H3BloomFilter::writeBit(const int index, const int value)
{
}

int
H3BloomFilter::getTotalCount()
{
    int count = 0;

    for (int i = 0; i < m_filter_size; i++) {
        count += m_filter[i];
    }
    return count;
}

void
H3BloomFilter::print(ostream& out) const
{
}

int
H3BloomFilter::get_index(Addr addr, int i)
{
    uint64_t x = makeLineAddress(addr);
    // uint64_t y = (x*mults_list[i] + adds_list[i]) % primes_list[i];
    int y = hash_H3(x,i);

    if (isParallel) {
        return (y % m_par_filter_size) + i*m_par_filter_size;
    } else {
        return y % m_filter_size;
    }
}

int
H3BloomFilter::hash_H3(uint64_t value, int index)
{
    uint64_t mask = 1;
    uint64_t val = value;
    int result = 0;

    for (int i = 0; i < 64; i++) {
        if (val&mask) result ^= H3[i][index];
        val = val >> 1;
    }
    return result;
}

