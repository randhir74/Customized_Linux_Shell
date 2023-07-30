#include <bits/stdc++.h>
using namespace std;

int helper(int n, vector<int> &dp)
{
    //   cout << "In start of helper function" << endl;
    if (n == 1)
    {
        return 1;
    }
    // cout<<"In middle of helper function"<<endl;
    if (dp[n] != -1)
    {
        return dp[n];
    }
    // cout<<"In end of helper function"<<endl;
    int ans = 0;
    for (int i = n - 1; i >= 1; i--)
    {
        if (n % i == 0)
        {
            ans += helper(i, dp);
        }
    }
    //   cout << n << ' ' << ans << endl;
    dp[n] = ans;
    return ans;
}

int main()
{
    int n;
    cin >> n;
    vector<int> dp(n + 1, -1);
    int ans = helper(n, dp);
    //   cout << "Hello after helper called" << endl;
    cout << ans << endl;
    return ans;
}
