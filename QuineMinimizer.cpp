#include <cctype>
#include <algorithm>
#include <utility>
#include <sstream>
#include <map>
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;



//qm
string bitsMeChange(int x, int n)
{
    
    string s(n, '0');
    //changing into bits
    for (int i = n - 1; i >= 0; --i)
    {
        s[i] = (x & 1) ? '1' : '0';
        //cout << s[i]
        x >>= 1;
    }
    return s;
}
struct Implicant
{
    string pattern;
    set<int> minterms;
    bool used = false;
    bool fromDontCare = false;

    Implicant(string p, set<int> m, bool d = false)
        : pattern(move(p)), minterms(move(m)), fromDontCare(d) {}
};

pair<string, bool> combineImp(const string &a, const string &b)
{
    int alag = 0;
    string out;
    for (int i = 0; i < (int)a.size(); ++i)
    {
        if (a[i] == b[i])
            out.push_back(a[i]);
        else
        {
            ++alag;
            out.push_back('-');
            if (alag > 1)
                return {"", false};
        }
    }
    return {out, alag == 1};
}

int literalCount(const string &s)
{
    int c = 0;
    for (char ch : s)
        if (ch != '-')
            ++c;
    return c;
}

vector<Implicant> findPrimeImplicants(vector<int> minterms, vector<int> dontcares, int n)
{
    vector<int> all(minterms.begin(), minterms.end());
    all.insert(all.end(), dontcares.begin(), dontcares.end());
    sort(all.begin(), all.end());
    all.erase(unique(all.begin(), all.end()), all.end());

    //checking one case
    // vector<Implicant> primeImplicants;
    // while (!groups.empty())
    // {
    //     map<int, vector<Implicant>> newGroups;
    //     for (auto it = groups.begin(); it != prev(groups.end()); ++it)
    //     {
    //         int g1 = it->first, g2 = next(it)->first;
    //         for (auto &imp1 : groups[g1])
    //         {
    //             for (auto &imp2 : groups[g2])
    //             {
    //                 auto [combPattern, ok] = combineImp(imp1.pattern, imp2.pattern);
    //                 if (ok)
    //                 {
    map<int, vector<Implicant>> groups;
    for (int t : all)
    {

        string bits = bitsMeChange(t, n);
        int ones = count(bits.begin(), bits.end(), '1');
        groups[ones].push_back(Implicant(bits, {t}, find(dontcares.begin(), dontcares.end(), t) != dontcares.end()));
    }

    vector<Implicant> primeImplicants;
    while (!groups.empty())
    {
        map<int, vector<Implicant>> newGroups;
        for (auto it = groups.begin(); it != prev(groups.end()); ++it)
        {
            int g1 = it->first, g2 = next(it)->first;
            for (auto &imp1 : groups[g1])
            {
                for (auto &imp2 : groups[g2])
                {
                    auto [combPattern, ok] = combineImp(imp1.pattern, imp2.pattern);
                    if (ok)
                    {
                        set<int> uni = imp1.minterms;
                        uni.insert(imp2.minterms.begin(), imp2.minterms.end());
                        newGroups[g1].push_back(Implicant(combPattern, uni, imp1.fromDontCare && imp2.fromDontCare));
                    }
                }
            }
        }
        for (auto &[_, group] : groups)
        {
            for (auto &imp : group)
            {

                primeImplicants.push_back(imp);
            }
        }
        groups = move(newGroups);
    }
    return primeImplicants;
}


vector<Implicant> chhotaQM(vector<int> minterms, vector<int> dontcares, int n)
{
    auto pprriimmee = findPrimeImplicants(minterms, dontcares, n);

    map<int, vector<int>> chart;
    for (int i = 0; i < (int)pprriimmee.size(); i++)
    {
        for (int m : pprriimmee[i].minterms)
        {
            if (find(minterms.begin(), minterms.end(), m) != minterms.end())
            {
                chart[m].push_back(i);
            }
        }
    }

    set<int> essential, covered;
    for (auto &[m, imps] : chart)
    {
        if (imps.size() == 1)
        {
            int pi = imps[0];
            essential.insert(pi);
            for (int mm : pprriimmee[pi].minterms)
                if (find(minterms.begin(), minterms.end(), mm) != minterms.end())
                    covered.insert(mm);
        }
    }

    while (covered.size() < minterms.size())
    {
        int bestPi = -1, bestCover = -1, bestLit = numeric_limits<int>::max();
        for (int i = 0; i < (int)pprriimmee.size(); i++)
        {
            if (essential.count(i))
                continue;
            int c = 0;
            for (int m : pprriimmee[i].minterms)
                if (find(minterms.begin(), minterms.end(), m) != minterms.end() && !covered.count(m))
                    ++c;
            if (c > bestCover || (c == bestCover && literalCount(pprriimmee[i].pattern) < bestLit))
            {
                bestPi = i;
                bestCover = c;
                bestLit = literalCount(pprriimmee[i].pattern);
            }
        }
        if (bestPi == -1)
            break;
        essential.insert(bestPi);
        for (int m : pprriimmee[bestPi].minterms)
            if (find(minterms.begin(), minterms.end(), m) != minterms.end())
                covered.insert(m);
    }

// vector<Implicant> chhotaQM(vector<int> minterms, vector<int> dontcares, int n)
// {
//     auto pprriimmee = findPrimeImplicants(minterms, dontcares, n);

//     map<int, vector<int>> chart;
//     for (int i = 0; i < (int)pprriimmee.size(); i++)
//     {
//         for (int m : pprriimmee[i].minterms)
//         {
//             if (find(minterms.begin(), minterms.end(), m) != minterms.end())
//             {
//                 chart[m].push_back(i);
//             }
//         }
//     }

//     set<int> essential, covered;
//     for (auto &[m, imps] : chart)
//     {
//         if (imps.size() == 1)
//         {
//             int pi = imps[0];
//             essential.insert(pi);
//             for (int mm : pprriimmee[pi].minterms)
//                 if (find(minterms.begin(), minterms.end(), mm) != minterms.end())
//                     covered.insert(mm);
//         }
//     }

    vector<Implicant> result;

    for (int i : essential)
    
        result.push_back(pprriimmee[i]);
        //cout << pprriimmee[i]
    return result;
}

string patternToExpr(const string &pattern, int n, const vector<char> &vars)
{
    string out;
    for (int i = 0; i < n; ++i)
    {
        if (pattern[i] == '-')
        //cout << pprriimmee[i]
            continue;
        if (pattern[i] == '1')
            out.push_back(vars[i]);
        else
        {
            //cout << pprriimmee[i]
            out.push_back(vars[i]);
            out.push_back('\'');
        }
    }
    if (out.empty())
        return "1";
    return out;
}

//q2

string toBinary(int num, int n)
{
    string s(n, '0');
    //cout << pprriimmee[i]
    for (int i = n - 1; i >= 0; --i)
    {
        if (num & 1)
            s[i] = '1';
        num >>= 1;
    }
    return s;
}

int alagBits(const string &a, const string &b)
//cout << pprriimmee[i]
{
    int alag = 0;
    for (int i = 0; i < (int)a.size(); ++i)
        if (a[i] != b[i])
            ++alag;
    return alag;
}

string combine(const string &a, const string &b)
{
    //cout << pprriimmee[i]
    string res = a;
    for (int i = 0; i < (int)a.size(); ++i)
        if (a[i] != b[i])
            res[i] = '-';
    return res;
}

void termExpansion(const string &s, int idx, string cur, vector<int> &res)
{
    if (idx == (int)s.size())
    //cout << pprriimmee[i]
    {
        res.push_back(stoi(cur, nullptr, 2));
        return;
    }
    if (s[idx] == '-')
    //cout << pprriimmee[i]
    {
        termExpansion(s, idx + 1, cur + "0", res);
        termExpansion(s, idx + 1, cur + "1", res);
    }
    else
    {
        termExpansion(s, idx + 1, cur + s[idx], res);
    }
}

string termToExpr(const string &s, int n)
{
    string vars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string ans = "";
    for (int i = 0; i < n; ++i)
    {
        if (s[i] == '1')
            ans += vars[i];
        else if (s[i] == '0')
            ans += vars[i] + string("'");
    }
    return ans;
}

bool absorbs(const string &a, const string &b)
{
    for (int i = 0; i < (int)a.size(); ++i)
    {
        if (a[i] != '-' && a[i] != b[i])
            return false;
    }
    return true;
}

vector<set<string>> multiply(const vector<set<string>> &a, const vector<set<string>> &b)
{
    vector<set<string>> res;
    for (auto &x : a)
    {
        for (auto &y : b)
        {
            set<string> merged = x;
            merged.insert(y.begin(), y.end());
            res.push_back(merged);
        }
    }
    return res;
}

void truthtabled(const vector<int> &minterms, int n, const vector<string> &patterns)
{
    int total = 1 << n;
    cout << "\nTRUTH TABLE (inputShow, orgF, minF):\n";
    cout << "Input min\n";


    for (int v = 0; v < total; ++v)
    {
        string bits = bitsMeChange(v, n);
        int orig = (find(minterms.begin(), minterms.end(), v) != minterms.end()) ? 1 : 0;
        int mini = 0;
        for (auto &p : patterns)
        {
            bool ok = true;
            for (int i = 0; i < n; ++i)
            {
                if (p[i] == '-')
                    continue;
                if (p[i] != bits[i])
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                mini = 1;
                break;
            }
        }
        cout << bits << " " << orig << " " << mini << "\n";
    }
}

void HeuristicChalao()
{
    string form;
    int n;
    cout << "Enter which type of form (SOP/POS): ";
    cin >> form;
    cout << "How many number of variables: ";
    cin >> n;

    int m;
    vector<int> terms, dontcares;
    if (form == "SOP")
    {
        cout << "How many number of minterms: ";
        cin >> m;
        cout << "Provide minterms: ";
        terms.resize(m);
        for (int i = 0; i < m; i++)
            cin >> terms[i];
    }
    else
    {
        cout << "How many number of maxterms: ";
        cin >> m;
        cout << "Provide maxterms: ";
        //cout << pprriimmee[i]
        terms.resize(m);
        for (int i = 0; i < m; i++)
            cin >> terms[i];
        //cout << pprriimmee[i]
        vector<int> all;
        for (int i = 0; i < (1 << n); ++i)
            all.push_back(i);
        set<int> maxset(terms.begin(), terms.end());
        terms.clear();
        for (int i : all)
        //cout << pprriimmee[i]
            if (!maxset.count(i))
                terms.push_back(i);
    }

    cout << "How many number of don't-cares: ";
    cin >> m;
    if (m > 0)
    {
        cout << "How many don't-care terms: ";
        dontcares.resize(m);
        for (int i = 0; i < m; i++)
            cin >> dontcares[i];
    }

    auto minimized = chhotaQM(terms, dontcares, n);
    vector<char> vars;
    for (int i = 0; i < n; ++i)
        vars.push_back('A' + i);

    cout << "Minimized Expression would be-Heuristic SOP): ";
    vector<string> minimizedPatterns;
    for (int i = 0; i < (int)minimized.size(); i++)
    {
        if (i > 0)
            cout << " + ";
        cout << patternToExpr(minimized[i].pattern, n, vars);
        minimizedPatterns.push_back(minimized[i].pattern);
    }
    cout << "\n";


    truthtabled(terms, n, minimizedPatterns);
}

//q2P

void petrickChalao()
{
    int n, m, d;
    string form;
    cout << "Enter the type of the form (SOP/POS): ";
    cin >> form;

    cout << "How many number of variables: ";
    cin >> n;

    cout << "How many number of terms: ";
    cin >> m;
    vector<int> terms(m);
    cout << "Enter terms: ";
    for (int i = 0; i < m; ++i)
        cin >> terms[i];

    cout << "How many don't-cares: ";
    cin >> d;
    vector<int> dontcares(d);
    if (d > 0)
    {
        cout << "Enter don't-care terms: ";
        for (int i = 0; i < d; ++i)
            cin >> dontcares[i];
    }

    vector<int> minterms;
    if (form == "POS" || form == "pos")
    {
        int total = 1 << n;
        set<int> termSet(terms.begin(), terms.end());
        for (int i = 0; i < total; ++i)
            if (termSet.find(i) == termSet.end())
                minterms.push_back(i);
    }
    else
    {
        minterms = terms;
    }

    vector<string> allTerms;
    for (int mt : minterms)
        allTerms.push_back(toBinary(mt, n));
    for (int dc : dontcares)
        allTerms.push_back(toBinary(dc, n));

    set<string> pprriimmee;
    vector<string> termsList = allTerms;
    bool changed = true;
    while (changed)
    {
        changed = false;
        set<string> next;
        vector<bool> used(termsList.size(), false);

        for (int i = 0; i < (int)termsList.size(); ++i)
        {
            for (int j = i + 1; j < (int)termsList.size(); ++j)
            {
                if (alagBits(termsList[i], termsList[j]) == 1)
                {
                    next.insert(combine(termsList[i], termsList[j]));
                    used[i] = used[j] = true;
                    changed = true;
                }
            }
        }
        for (int i = 0; i < (int)termsList.size(); ++i)
            if (!used[i])
                pprriimmee.insert(termsList[i]);
        termsList.assign(next.begin(), next.end());
    }

    map<string, vector<int>> pcover;
    for (auto p : pprriimmee)
    {
        vector<int> covered;
        termExpansion(p, 0, "", covered);
        for (int mt : minterms)
            if (find(covered.begin(), covered.end(), mt) != covered.end())
                pcover[p].push_back(mt);
    }

    set<int> covered;
    vector<string> essentials;
    for (int mt : minterms)
    {
        int cnt = 0;
        string chosen = "";
        for (auto &kv : pcover)
        {
            if (find(kv.second.begin(), kv.second.end(), mt) != kv.second.end())
            {
                cnt++;
                chosen = kv.first;
            }
        }
        if (cnt == 1 && find(essentials.begin(), essentials.end(), chosen) == essentials.end())
        {
            essentials.push_back(chosen);
            for (int x : pcover[chosen])
                covered.insert(x);
        }
    }

    vector<int> uncovered;
    for (int mt : minterms)
        if (!covered.count(mt))
            uncovered.push_back(mt);

    vector<set<string>> petrick = {{}};
    for (int mt : uncovered)
    {
        vector<set<string>> choices;
        for (auto &kv : pcover)
        {
            if (find(kv.second.begin(), kv.second.end(), mt) != kv.second.end())
            {
                choices.push_back({kv.first});
            }
        }
        petrick = multiply(petrick, choices);
    }

    vector<string> finalCover = essentials;
    if (!petrick.empty())
    {
        int bestSize = numeric_limits<int>::max();
        vector<string> bestSet;
        for (auto &s : petrick)
        {
            int sz = s.size();
            if (sz < bestSize)
            {
                bestSize = sz;
                bestSet.assign(s.begin(), s.end());
            }
        }
        finalCover.insert(finalCover.end(), bestSet.begin(), bestSet.end());
    }
    set<string> uniq(finalCover.begin(), finalCover.end());
    vector<string> uniqueCover(uniq.begin(), uniq.end());
    vector<string> minimized;
    for (int i = 0; i < (int)uniqueCover.size(); ++i)
    {
        bool redundant = false;
        for (int j = 0; j < (int)uniqueCover.size(); ++j)
        {
            if (i != j && absorbs(uniqueCover[j], uniqueCover[i]))
            {
                redundant = true;
                break;
            }
        }
        if (!redundant)
            minimized.push_back(uniqueCover[i]);
    }
    finalCover = minimized;
    cout << "\nMinimized SOP Expression would be-Petrick's method): ";
    for (int i = 0; i < (int)finalCover.size(); ++i)
    {
        if (i)
            cout << " + ";
        cout << termToExpr(finalCover[i], n);
    }
    cout << endl;
    truthtabled(minterms, n, finalCover);
}

//q3

struct Gate
{
    string name;
    string in1, in2;
    string out;
};

void GateRealizerChalao()
{
    int gatecnt = 0;
     auto newgate = [&]()
    {
        return string("G") + to_string(++gatecnt);
    };
    map<string, string> complementer;
    vector<Gate> final;

   

    function<string(const string &)> invertKardo = [&](const string &lit) -> string
    {
        if (complementer.count(lit))
            return complementer[lit];
        string g = newgate();
        string out = lit + "'";
        final.push_back({g, lit, lit, out});
        complementer[lit] = out;
        return out;
    };

    function<string(vector<string> &)> nandProduct = [&](vector<string> &lits) -> string
    {
        if (lits.empty())
            return string();
        if (lits.size() == 1)
            return lits[0];
        if (lits.size() == 2)
        {
            string g = newgate();
            string out = string("N") + to_string(gatecnt);
            final.push_back({g, lits[0], lits[1], out});
            return out;
        }
        string g1 = newgate();
        string temp1 = string("T") + to_string(gatecnt);
        final.push_back({g1, lits[0], lits[1], temp1});

        string g2 = newgate();
        string temp2 = string("T") + to_string(gatecnt);
        final.push_back({g2, temp1, temp1, temp2});

        string current = temp2;
        for (size_t i = 2; i < lits.size(); ++i)
        {
            string g3 = newgate();
            string temp3 = string("T") + to_string(gatecnt);
            final.push_back({g3, current, lits[i], temp3});
            current = temp3;
        }
        return current;
    };

    function<string(const string &)> koiInvert = [&](const string &lit) -> string
    {
        if (!lit.empty() && lit.back() == '\'')
            return lit.substr(0, lit.size() - 1);
        return invertKardo(lit);
    };

    function<string(vector<string> &)> norProduct = [&](vector<string> &lits) -> string
    {
        if (lits.empty())
            return string();
        if (lits.size() == 1)
            return lits[0];
        string a = lits[0];
        for (size_t i = 1; i < lits.size(); ++i)
        {
            string na = koiInvert(a);
            string nb = koiInvert(lits[i]);
            string g = newgate();
            string out = string("T") + to_string(gatecnt);
            final.push_back({g, na, nb, out});
            a = out;
        }
        return a;
    };

    function<string(vector<string> &)> sumNor = [&](vector<string> &prods) -> string
    {
        if (prods.empty())
            return string();
        if (prods.size() == 1)
            return prods[0];
        string cur = prods[0];
        for (size_t i = 1; i < prods.size(); ++i)
        {
            string g1 = newgate();
            string t = string("T") + to_string(gatecnt);
            final.push_back({g1, cur, prods[i], t});
            if (i == prods.size() - 1)
            {
                string g2 = newgate();
                final.push_back({g2, t, t, "F"});
                return string("F");
            }
            else
            {
                string g2 = newgate();
                string t2 = string("T") + to_string(gatecnt);
                final.push_back({g2, t, t, t2});
                cur = t2;
            }
        }
        return string();
    };

    function<string(vector<string> &)> sumNandPoscheck = [&](vector<string> &lits) -> string
    {
        if (lits.empty())
            return string();
        if (lits.size() == 1)
            return lits[0];
        vector<string> inv;
        for (auto &l : lits)
            inv.push_back(koiInvert(l));
        string andOut = nandProduct(inv);
        return invertKardo(andOut);
    };

    function<string(vector<vector<string>> &)> buildPOSNand = [&](vector<vector<string>> &sumTerms) -> string
    {
        vector<string> sumOutputs;
        for (auto &term : sumTerms)
            sumOutputs.push_back(sumNandPoscheck((vector<string> &)term));
        if (sumOutputs.size() == 1)
            return sumOutputs[0];
        string g1 = newgate();
        string temp1 = string("T") + to_string(gatecnt);
        final.push_back({g1, sumOutputs[0], sumOutputs[1], temp1});
        string g2 = newgate();
        string temp2 = string("T") + to_string(gatecnt);
        final.push_back({g2, temp1, temp1, temp2});
        string current = temp2;
        for (size_t i = 2; i < sumOutputs.size(); ++i)
        {
            string g3 = newgate();
            string temp3 = string("T") + to_string(gatecnt);
            final.push_back({g3, current, sumOutputs[i], temp3});
            current = temp3;
        }
        return current;
    };

    function<string(vector<string> &)> norPoskaSUM = [&](vector<string> &lits) -> string
    {
        if (lits.empty())
            return string();
        if (lits.size() == 1)
            return lits[0];
        string cur = lits[0];
        for (size_t i = 1; i < lits.size(); ++i)
        {
            string na = koiInvert(cur);
            string nb = koiInvert(lits[i]);
            string g = newgate();
            string out = string("T") + to_string(gatecnt);
            final.push_back({g, na, nb, out});
            cur = out;
        }
        return invertKardo(cur);
    };

    function<string(vector<vector<string>> &)> buildPOSNor = [&](vector<vector<string>> &sumTerms) -> string
    {
        vector<string> sumOutputs;
        for (auto &term : sumTerms)
            sumOutputs.push_back(norPoskaSUM((vector<string> &)term));
        if (sumOutputs.size() == 1)
            return sumOutputs[0];
        string cur = sumOutputs[0];
        for (size_t i = 1; i < sumOutputs.size(); ++i)
        {
            string g1 = newgate();
            string t = string("T") + to_string(gatecnt);
            final.push_back({g1, cur, sumOutputs[i], t});
            cur = t;
        }
        return cur;
    };

    string expr, mode, exprType;
    cout << "Which expression type (SOP or POS): ";
    cin >> exprType;
    for (char &c : exprType)
        c = toupper((unsigned char)c);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Give the expression:\n";
    getline(cin, expr);

    cout << "Enter which mode (NAND/NOR): ";
    cin >> mode;
    for (char &c : mode)
        c = toupper((unsigned char)c);

    expr.erase(remove(expr.begin(), expr.end(), ' '), expr.end());

    vector<string> productOutputs;
    string finalOutput;
    if (exprType == "POS")
    {
        vector<vector<string>> sumTerms;
        string temp = "";
        for (char c : expr)
        {
            if (c == '(')
                temp = "";
                // if (!temp.empty())
                // {
                //     stringstream ss(temp);
                //     string lit;
                //     vector<string> lits;
                //     while (getline(ss, lit, '+'))
                //      for(char c:exp)
                //     {
                //         lits.push_back(lit);
                //     }
                //     sumTerms.push_back(lits);
                // }
            else if (c == ')')
            {
                if (!temp.empty())
                {
                    stringstream ss(temp);
                    string lit;
                    vector<string> lits;
                    while (getline(ss, lit, '+'))
                    {
                        lits.push_back(lit);
                    }
                    sumTerms.push_back(lits);
                }
            }
            else
                temp += c;
        }

        if (mode == "NAND")
            finalOutput = buildPOSNand(sumTerms);
        else
            finalOutput = buildPOSNor(sumTerms);
    }
    else if (exprType == "SOP")
    {
        vector<string> terms;
        stringstream ss(expr);
        string item;
        while (getline(ss, item, '+'))
            terms.push_back(item);

        for (auto &term : terms)
        {
            vector<string> lits;
            for (size_t i = 0; i < term.size();)
            {
                char c = term[i];
                if (isalpha((unsigned char)c))
                {
                    if (i + 1 < term.size() && term[i + 1] == '\'')
                    {
                        string base(1, c);
                        string inv = invertKardo(base);
                        lits.push_back(inv);
                        i += 2;
                    }
                    else
                    {
                        lits.push_back(string(1, c));
                        ++i;
                    }
                }
                else
                    ++i;
            }

            if (mode == "NAND")
                productOutputs.push_back(nandProduct(lits));
            else
                productOutputs.push_back(norProduct(lits));
        }

        if (mode == "NAND")
        {
            if (productOutputs.size() == 1)
                finalOutput = productOutputs[0];
            else if (productOutputs.size() == 2)
            {
                string g = newgate();
                final.push_back({g, productOutputs[0], productOutputs[1], "F"});
                finalOutput = "F";
            }
            else
            {
                string g1 = newgate();
                string temp1 = string("T") + to_string(gatecnt);
                final.push_back({g1, productOutputs[0], productOutputs[1], temp1});

                string g2 = newgate();
                string temp2 = string("T") + to_string(gatecnt);
                final.push_back({g2, temp1, temp1, temp2});

                string current = temp2;
                for (size_t i = 2; i < productOutputs.size(); ++i)
                {
                    string g3 = newgate();
                    string temp3 = string("T") + to_string(gatecnt);
                    final.push_back({g3, current, productOutputs[i], temp3});
                    current = temp3;
                }
                final.push_back({"", current, "", "F"});
                finalOutput = "F";
            }
        }
        else
        {
            finalOutput = sumNor(productOutputs);
        }
    }
    

    cout << "\n"
         << mode << " final:\n";
    for (const auto &g : final)
    {
        if (g.name.empty())
        {
            cout << "Final output signal will be: " << g.out << "\n";
        }
        else
        {
            cout << g.name << ": " << mode << "(" << g.in1 << "," << g.in2 << ") -> " << g.out << "\n";
        }
    }

    cout << "Final Ans: " << finalOutput << "\n";

    int num = (int) final.size();
    if (!final.empty() && final.back().name.empty())
        --num;
    cout << "For that " << num << " gates will be used" << endl;
}

//Main menu

int main()
{
    cout << "Question no:\n";
    cout << "1 for Quine-McCluskey-Heuristic Method)\n";
    cout << "2 for Quine-McCluskey-Petrick's Method)\n";
    cout << "3 for Gate-level realization (NAND/NOR only)\n";
    cout << "Enter the choice accordingly";
    int choice;
    if (!(cin >> choice))
        return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 1)
        HeuristicChalao();
    else if (choice == 2)
        petrickChalao();
    else if (choice == 3)
        GateRealizerChalao();
    else
        cout << "Wrong choice\n";

    return 0;
}
