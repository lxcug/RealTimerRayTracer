# Microfacet Theory

## Observation of Microfacet

<img src="https://raw.githubusercontent.com/lxcug/imgs/main/20240419104705.png"  />

(a) Masking: 散射光被微表面遮挡。

(b) Shadowing: 光线被微表面遮挡导致没有照射到视线所观察的微表面。

(c) Interreflection: 光线在微表面之间散射最终进入眼睛。

## Microfacet Distribution

考虑在法线$n$所在半球空间对微表面法线方向$\omega_m$积分，记$D(\omega_m)$垂直于$\omega_m$​所对应的微表面的面积，

<img src="https://raw.githubusercontent.com/lxcug/imgs/main/IMG_6578.JPG" style="zoom: 33%;" />
$$
\int_\Omega D(\omega_m) (\omega_m, n) \mathrm d \omega = \int_\Omega D(\omega_m) \cos\theta_m \mathrm d \omega = 1 \tag{1} \label{1}
$$
常用的$D(\omega_m)$为**GGX分布**，其解析解为，
$$
D_{GGX}(\omega_m, \alpha_x, \alpha_y) = \frac{1}{\pi \alpha_x \alpha_y \cos^4\theta_m \left( 1 + \tan^2\theta_m\left( \frac{\cos^2\phi_m}{\alpha_x^2} + \frac{\sin^2\phi_m}{\alpha_y^2} \right) \right)^2}
$$
其中$\omega_m = (\theta_m, \phi_m)$分别代表天顶角zenith和方位角azimuth。

上式针对各向异性材质(anisotropic)，对于各向同性材质(isotropic)，$\alpha = \alpha_x = \alpha_y$，上式退化为，
$$
D_{GGX}(\omega_m, \alpha) = \frac{1}{\pi \alpha^2 \cos^4\theta_m \left( 1 +  \frac{\tan^2\theta_m}{\alpha^2} \right)^2}
$$

## Masking Function

Masking为了考虑到散射光被微表面遮挡的情况，由于光路可逆，我们可以从视线被微表面遮挡的角度来考虑，Recall equation $\eqref{1}$, 我们用$G_1(\omega, \omega_m)$来表示从$\omega$方向观察以$\omega_m$为法线的微表面可见的比例，$0 \le G_1(\omega, \omega_m) \le 1$​,

![](https://raw.githubusercontent.com/lxcug/imgs/main/20240419115443.png)
$$
\int_\Omega D(\omega_m) G_1(\omega, \omega_m) \max(0, \omega\cdot\omega_m) \mathrm d \omega_m = \omega \cdot n = \cos\theta \tag{2}\label{2}
$$
可以这么理解equation $\eqref{2}$，$D(\omega_m)$为微表面垂直于$\omega_m$方向上的面积，从微表面角度来看，从$\omega$方向可以看到的微表面面积$\mathrm d s$为$D(\omega_m)$在垂直于$\omega$平面的投影并乘上$G_1$项，
$$
\mathrm ds = \int_\Omega D(\omega_m) \max(0, \omega \cdot \omega_m) \mathrm d \omega_m
$$
$\mathrm dA$为$D(\omega_m)$投影到垂直于$n$平面的面积，从宏观角度来看，$\mathrm ds = \mathrm dA \cos\theta$。



仅仅靠equation $\eqref{2}$无法得到一个确定的$G_1$项，需要更多约束，这里引入一个假设，所有微表面的高度和法线相互独立，即微表面不是一个连接的整体，而是漂浮的一个个平面，彼此互不连接，这样masking就不依赖于微表面法线$\omega_m$​，
$$
G_1(\omega) \int_\Omega D(\omega_m) \max(0, \omega\cdot \omega_m) \mathrm d\omega_m = \cos\theta 
$$

$$
\text{Thus, } \ \ G_1(\omega) = \frac{\cos\theta}{\int_\Omega D(\omega_m) \max(0, \omega\cdot \omega_m) \mathrm d\omega_m}  \tag{3} \label{3}
$$

Equation $\eqref{3}$是Smith近似，$G_1$的Smith近似有解析解，
$$
G_1(w) = \frac{1}{1 + \Lambda(\omega)}
$$
如果$D(\omega_m)$使用GGX分布，则
$$
\Lambda(\omega) = \frac{\sqrt{1 + \alpha^2\tan^2\theta} -1}{2}
$$
where $\alpha = \sqrt{\alpha_x^2\cos^2\phi + \alpha_y^2\sin^2\phi}$.

## Masking-Shadowing Function

Shadowing Function和Masking Function类似，只不过参数为光线入射方向而不是观察方向，

如果假设Masking和Shadowing相互独立，Masking-Shadowing Function可以写作，
$$
G(\omega_o, \omega_i) = G_1(\omega_o)\cdot G_1(\omega_i)
$$
但是**这种假设会overestimate Masking和Shadowing**，造成某些区域变暗，PBRT没有采用上述假设，而是基于这样一个观察，**elevation越大(for both $\omega_o, \omega_i$)，$G(\omega_o, \omega_i)$就越小，即被遮挡的比例越小**，
$$
G(\omega_o, \omega_i) = \frac{1}{1 + \Lambda(\omega_o) + \Lambda(\omega_i)}
$$

## Sampling

Recall equation $\eqref{2}$, 将等式两边同时除上$\cos\theta$,
$$
\int_\Omega \frac{G_1(\omega)}{\cos\theta} D(\omega_m) \max(0, \omega \cdot\omega_m) \mathrm d\omega_m = 1
$$
记
$$
D_\omega(\omega_m) = \frac{G_1(\omega)}{\cos\theta} D(\omega_m) \max(0, \omega \cdot\omega_m) \tag{3}\label{pdf of wm}
$$
$D_\omega(\omega_m)$就是以$\omega$​为观察方向的微表面材质的法线分布概率密度函数，通过$D_\omega(\omega_m)$可以计算出特定观察方向上微表面法线$w_m$的概率密度，我们通过某种方法采样出微表面法线方向$\omega_m$​​，就可以得到incident direction `wi = reflect(-wo, wm)`，因为每个微表面反射视为镜面反射，**注意由于我们是采样微表面法线$w_m$进而推导出incident direction $\omega_i$，所以我们还要根据$\omega_m$的概率密度推出$\omega_i$的概率密度**，为了由$\omega_m$的pdf推出$\omega_i$的pdf，我们需要推出$\frac{\mathrm \omega_i}{\mathrm d\omega_m}$，

> 在法线空间下$n=(0, 0, 1)$，$\omega_i=(\theta_i, \phi_i), \omega_i=(\theta_o, \phi_o), \omega_m=(\theta_m, \phi_m)$，
> $$
> \omega_i = -\omega_o + 2(\omega_m, \omega_o)\omega_m\\
> \omega_m = \frac{\omega_i + \omega_o}{|| \omega_i + \omega_o ||}
> $$
> <img src="https://raw.githubusercontent.com/lxcug/imgs/main/20240421142913.png" style="zoom:150%;" />
> $$
> \frac{\mathrm d\omega_m}{\mathrm d\omega_i} = \frac{\sin\theta_m \mathrm d\theta_m \mathrm d\phi_m}{\sin\theta_i \mathrm d\theta_i \mathrm d\phi_i} \tag{4}\label{4}
> $$
> 在法线空间下我们不好得到$\frac{\mathrm d\omega_m}{\mathrm d\omega_i}$，考虑将equation $\eqref{4}$中的所有方向和角度转换到以$\omega_o$为z轴的空间中，在此空间下$\theta_i = 2\theta_m, \phi_i=\phi_m$，equation $\eqref{4}$可以进行化简，
> $$
> \begin{aligned}
> \frac{\mathrm d\omega_m}{\mathrm d\omega_i} &= \frac{\sin\theta_m \mathrm d\theta_m \mathrm d\phi_m}{\sin\theta_i \mathrm d\theta_i \mathrm d\phi_i} \\
> &= \frac{\sin\theta_m \mathrm d\theta_m }{\sin2\theta_m 2\mathrm d\theta_m} \\
> &= \frac{1}{4\cos\theta_m} \\
> &= \frac{1}{4(\omega_i, \omega_m)}\\
> &= \frac{1}{4(\omega_o, \omega_m)}
> \end{aligned}
> $$
> 所以$\omega_i$的pdf为，
> $$
> p(\omega_i) = p(\omega_m)\frac{\mathrm d\omega_m}{\mathrm d\omega_i} = \frac{D_\omega(\omega_m)}{4(\omega_i, \omega_m)}
> $$
> 





TODO: Sampling in PBRT

![](https://raw.githubusercontent.com/lxcug/imgs/main/20240419180119.png)



## BRDF of Microfacets



> 注意以上公式在推导时都默认在法线空间，即法线方向为$(0, 0, 1)$，在函数中传入的参数一般是世界空间的方向，因此在代码实现中做了一个世界空间到法线空间的转换。