## BxDF Interface

```c++
class BxDF {
public:
    /*!
     * Sample a incident direction wi and eval f and pdf, pack the data into BxDFSample.
     * @param wo
     * @return
     */
    [[nodiscard]] virtual BxDFSample
    sampleF(const glm::vec3 &wo, const glm::vec3 &normal) const = 0;

    virtual glm::vec3 evalLightTransport(const glm::vec3 &wi, const glm::vec3 &wo,
                                           const glm::vec3 &normal) const = 0;
};
```

BxDF定义了两个接口，`sampleF`根据$\omega_o$和normal采样$\omega_i$，并将light transport写入BxDFSample结构体中，

`evalLightTransport`根据$\omega_i, \omega_o$和normal确定light transport的值，

> FYI:
> $$
> \begin{aligned}
> L_o(p, \omega_o) &= \int_{\Omega} L_i(p, \omega_i, \omega_o) f \cos\theta \mathrm d \omega \\
> &= \frac{1}{N} \sum_{i=0}^N \frac{L_i f \cos\theta}{pdf}
> \end{aligned}
> $$
> 公式里的$\frac{f\cos\theta}{pdf}$即为light transport，不采用pbrt中将f和pdf分开计算是因为 f/pdf 在大多数情况下可以消掉$\cos\theta$和$\pi$。

## Diffuse

### Cosine Weighted Hemisphere Sampling





## Specular

只有`wi = reflect(-wo, normal)`方向上才会有光线入射，可以理解为从Dirac Delta Distribution采样$\omega_i$，

